#ifndef LIB_RATSS_PROJECT_SN_h
#define LIB_RATSS_PROJECT_SN_h
#pragma once

#include <libratss/constants.h>
#include <libratss/Calc.h>
#include <libratss/enum.h>
#include "internal/SkipIterator.h"
#include <assert.h>

namespace LIB_RATSS_NAMESPACE {

class ProjectSN {
public:
	typedef enum {
		ST_NONE=0x0,
		ST_SPHERE=0x1, //snap point on sphere
		ST_PLANE=0x2, //snap point on plane
		ST_CF=0x4, //snap by continous fraction
		ST_FX=0x8, //snap by fix point
		ST_FL=0x10, //snap by floating point
		ST_JP=0x20, // jacobi perron
		ST_NORMALIZE=0x40
	} SnapType;
public:
	static std::string toString(SnapType st);
public:
	template<typename T_FT_ITERATOR>
	PositionOnSphere positionOnSphere(T_FT_ITERATOR begin, const T_FT_ITERATOR & end) const WARN_UNUSED_RESULT;
	
	///Projects the coordinates of begin->end onto new coordinates such that one coordinate is zero
	///If you want to reproject onto the sphere, then you need to store the return value
	template<typename T_FT_INPUT_ITERATOR, typename T_FT_OUTPUT_ITERATOR>
	PositionOnSphere sphere2Plane(T_FT_INPUT_ITERATOR begin, const T_FT_INPUT_ITERATOR & end, T_FT_OUTPUT_ITERATOR out) const WARN_UNUSED_RESULT;
	
	template<typename T_FT_INPUT_ITERATOR, typename T_FT_OUTPUT_ITERATOR>
	void plane2Sphere(T_FT_INPUT_ITERATOR begin, const T_FT_INPUT_ITERATOR & end, PositionOnSphere pos, T_FT_OUTPUT_ITERATOR out) const;
public:
	///@param out an iterator accepting mpq_class
	template<typename T_INPUT_ITERATOR, typename T_OUTPUT_ITERATOR>
	void snap(T_INPUT_ITERATOR begin, T_INPUT_ITERATOR end, T_OUTPUT_ITERATOR out, int snapType, int eps = -1) const;
public:
	inline const Calc & calc() const { return m_calc; }
private:
	template<typename T_FT>
	inline T_FT add(const T_FT & a, const T_FT & b) const { return calc().add(a,b); }

	template<typename T_FT>
	inline T_FT sub(const T_FT & a, const T_FT & b) const { return calc().sub(a,b); }

	template<typename T_FT>
	inline T_FT mult(const T_FT & a, const T_FT & b) const { return calc().mult(a,b); }
	
	template<typename T_FT>
	inline T_FT div(const T_FT & a, const T_FT & b) const { return calc().div(a, b); }
private:
	Calc m_calc;
};

} //end namespace LIB_RATSS_NAMESPACE

//definitions

namespace LIB_RATSS_NAMESPACE {

template<typename T_FT_ITERATOR>
PositionOnSphere ProjectSN::positionOnSphere(T_FT_ITERATOR begin, const T_FT_ITERATOR & end) const {
	using std::iterator_traits;
	using value_type = typename iterator_traits<T_FT_ITERATOR>::value_type;
	if (begin == end) {
		return SP_INVALID;
	}
	int posIndex = -1;
	int posSign = 0;
	value_type v( std::numeric_limits<uint32_t>::min() );
	for(int p(1);begin != end; ++begin, ++p) {
		if (*begin > v) { //base vector (0...,1,...0)
			posIndex = p;
			posSign = 1;
			v = *begin;
		}
		else if ((-(*begin)) > v) { //base vector (0...,-1,...0)
			posIndex = p;
			posSign = -1;
			v = -*begin;
		}
	}
	assert(posIndex > 0 && posSign != 0);
	return (PositionOnSphere) (posIndex*posSign);
}


template<typename T_FT_INPUT_ITERATOR, typename T_FT_OUTPUT_ITERATOR>
PositionOnSphere ProjectSN::sphere2Plane(T_FT_INPUT_ITERATOR begin, const T_FT_INPUT_ITERATOR & end, T_FT_OUTPUT_ITERATOR out) const {
	using std::iterator_traits;
	using FT = typename iterator_traits<T_FT_INPUT_ITERATOR>::value_type;
	if (begin == end) {
		return SP_INVALID;
	}
	PositionOnSphere pos = positionOnSphere(begin, end);
	int projCoord = abs((int) pos); //starts from 1
	//first get the value of our projection coordinate
	FT projVal = *std::next(begin, projCoord-1);
	FT denom;
	if (pos < 0) {
		denom = sub(FT(1), projVal);
	}
	else {
		denom = add(FT(1), projVal);
	}
	
	T_FT_INPUT_ITERATOR it(begin);
	for(int i(1); i < projCoord; ++i, ++it, ++out) {
		*out = div(*it, denom);
	}
	//now comes the projection coordinate
	*out = div(FT(0), denom); //this makes sure that for mpfr::mpreal *out has the same precision as denom
	++out;
	++it;
	//and the rest
	for( ; it != end; ++it, ++out) {
		*out = div(*it, denom);
	}
	
	return pos;
}

template<typename T_FT_INPUT_ITERATOR, typename T_FT_OUTPUT_ITERATOR>
void ProjectSN::plane2Sphere(T_FT_INPUT_ITERATOR begin, const T_FT_INPUT_ITERATOR & end, PositionOnSphere pos, T_FT_OUTPUT_ITERATOR out) const {
	using std::iterator_traits;
	using std::distance;
	using FT = typename iterator_traits<T_FT_INPUT_ITERATOR>::value_type;
	if (pos == SP_INVALID) {
		return;
	}
	FT denom(1);
	int projCoord = abs((int) pos); //starts from 1
	assert(projCoord <= distance(begin, end));
	{
		T_FT_INPUT_ITERATOR it(begin);
		for(int i(1); i < projCoord; ++i, ++it) {
			denom = add(denom, mult(*it, *it) );
		}
		//now comes the projection coordinate, skip it
		++it;
		//and the rest
		for( ; it != end; ++it) {
			denom = add(denom, mult(*it, *it) );
		}
	}
	{
		T_FT_INPUT_ITERATOR it(begin);
		for(int i(1); i < projCoord; ++i, ++it, ++out) {
			*out = div<FT>(2 * (*it), denom);
		}
		//and the projection coordinate
		*out = (std::signbit<int>(pos) ? 1 : -1) * div<FT>(denom - 2, denom);
		++out;
		assert(*it == FT(0));
		++it;
		//and the rest
		for( ; it != end; ++it, ++out) {
			*out = div<FT>(2 * (*it), denom);
		}
	}
}


template<typename T_INPUT_ITERATOR, typename T_OUTPUT_ITERATOR>
void ProjectSN::snap(T_INPUT_ITERATOR begin, T_INPUT_ITERATOR end, T_OUTPUT_ITERATOR out, int snapType, int significands) const {
	using input_ft = typename std::iterator_traits<T_INPUT_ITERATOR>::value_type;
	using std::distance;
	std::size_t dims = distance(begin, end);
	
	if (snapType & ST_NORMALIZE) {
		std::vector<input_ft> normalized(dims);
		calc().normalize(begin, end, normalized.begin());
		snap(normalized.begin(), normalized.end(), out, snapType & ~ST_NORMALIZE);
		return;
	}
	
	std::vector<mpq_class> coords_plane_pq(dims);
	PositionOnSphere pos;
	if (snapType & ST_SPHERE) {
		std::vector<mpq_class> coords_sphere_pq(dims);
		calc().toRational(begin, end, coords_sphere_pq.begin(), snapType, significands);
		pos = sphere2Plane(coords_sphere_pq.begin(), coords_sphere_pq.end(), coords_plane_pq.begin());
	}
	else if (snapType & ST_PLANE) {
		std::vector<mpfr::mpreal> coords_plane(dims);
		pos = sphere2Plane(begin, end, coords_plane.begin());
		//this fixes the eps guarantee at the cost of 2 more bits. This is independent of the number of bits
		//The question remains: why?
		if (significands > 0 && snapType & (ST_CF|ST_FX)) {
			significands += 2;
		}
		if (snapType & ST_JP) {
// 			std::vector<mpfr::mpreal> tmpInput;
// 			std::vector<mpq_class> tmpOutput(2);
// 			std::vector<int>::const_iterator;
// 			
// 			int skipDim = std::abs(pos)-1;
// 			for(int i(0); i < 3; ++i) {
// 				if (i != skipDim) {
// 					tmpInput.emplace_back( coords_plane.at(i));
// 				}
// 			}
// 			assert(tmpInput.size() == 2);
// 			calc().toRational(tmpInput.begin(), tmpInput.end(), tmpOutput.begin(), snapType, significands);
// 			for(int i(0), j(0); i < 3; ++i) {
// 				if (i == skipDim) {
// 					continue;
// 				}
// 				else {
// 					coords_plane_pq[i] = tmpOutput[j];
// 					++j;
// 				}
// 			}
			using SkipInputIterator = internal::SkipIterator<std::vector<mpfr::mpreal>::const_iterator>;
			using SkipOutputIterator = internal::SkipIterator<std::vector<mpq_class>::iterator>;
			calc().toRational(SkipInputIterator);
		}
		else {
			calc().toRational(coords_plane.begin(), coords_plane.end(), coords_plane_pq.begin(), snapType, significands);
		}
	}
	else {
		throw std::runtime_error("ratss::ProjectSN::snap: Unsupported snap type");
	}
	plane2Sphere(coords_plane_pq.begin(), coords_plane_pq.end(), pos, out);
}

} //end namespace LIB_RATSS_NAMESPACE

#endif