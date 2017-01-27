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

		ST_AUTO_CF=0x40, //add cf to auto snapping
		ST_AUTO_FX=0x80, //add fx to auto snapping
		ST_AUTO_FL=0x100, //add fl to auto snapping
		ST_AUTO_JP=0x200, //add jp to auto snapping
		ST_AUTO=0x400, //select snapping that produces the smallest denominators
		ST_AUTO_ALL=ST_AUTO|ST_AUTO_CF|ST_AUTO_FX|ST_AUTO_JP, //try all snappings and use the best one
		
		ST_AUTO_POLICY_MIN_SUM_DENOM=0x1000,
		ST_AUTO_POLICY_MIN_MAX_DENOM=0x2000,
		ST_AUTO_POLICY_MIN_TOTAL_LIMBS=0x4000,
		ST_AUTO_POLICY_MIN_SQUARED_DISTANCE=0x8000,
		ST_AUTO_POLICY_MIN_MAX_NORM=0x10000,
		
		ST_NORMALIZE=0x100000,
		//Do not use the values below!
		ST__INTERNAL_NUMBER_OF_SNAPPING_TYPES=4, //this effecivly defines the shift to get from ST_* to ST_AUTO_*
		ST__INTERNAL_AUTO_POLICIES=ST_AUTO_POLICY_MIN_SUM_DENOM|ST_AUTO_POLICY_MIN_MAX_DENOM|ST_AUTO_POLICY_MIN_TOTAL_LIMBS|ST_AUTO_POLICY_MIN_SQUARED_DISTANCE|ST_AUTO_POLICY_MIN_MAX_NORM,
		ST__INTERNAL_AUTO_ALL_WITH_POLICY=ST_AUTO_ALL|ST__INTERNAL_AUTO_POLICIES
	} SnapType;
	class SnapConfig {
	public:
		SnapConfig();
		SnapConfig(int st, int precision, int significands);
		SnapConfig(int st, int significands);
		SnapConfig(const SnapConfig & other);
		SnapConfig & operator=(const SnapConfig & other);
	public:
		int snapType() const;
		int precision(int dimensions) const;
		int significands(int dimensions) const;
	private:
		int m_st;
		int m_precision;
		int m_significands;
	};
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
	void snap(T_INPUT_ITERATOR begin, T_INPUT_ITERATOR end, T_OUTPUT_ITERATOR out, int snapType, int significands = -1) const;
	
	///@param out an iterator accepting mpq_class
	template<typename T_INPUT_ITERATOR, typename T_OUTPUT_ITERATOR>
	void snap(T_INPUT_ITERATOR begin, T_INPUT_ITERATOR end, T_OUTPUT_ITERATOR out, const SnapConfig & sc) const;
	
public:
	inline const Calc & calc() const { return m_calc; }
private:
	template<typename T_INPUT_ITERATOR, typename T_OUTPUT_ITERATOR>
	void snapNormalized(T_INPUT_ITERATOR begin, T_INPUT_ITERATOR end, T_OUTPUT_ITERATOR out, int snapType, int significands, std::size_t dims) const;
	template<typename T_ITERATOR>
	std::size_t summedDenomSize(T_ITERATOR begin, const T_ITERATOR& end) const;
	template<typename T_ITERATOR>
	std::size_t maxDenom(T_ITERATOR begin, const T_ITERATOR& end) const;
	template<typename T_ITERATOR>
	std::size_t limbCount(T_ITERATOR begin, const T_ITERATOR& end) const;
	template<typename T_ITERATOR_1, typename T_ITERATOR_2>
	mpq_class squaredDistance(T_ITERATOR_1 begin1, T_ITERATOR_2 begin2, const T_ITERATOR_1 & end1) const;
	template<typename T_ITERATOR_1, typename T_ITERATOR_2>
	mpq_class maxNorm(T_ITERATOR_1 begin1, T_ITERATOR_2 begin2, const T_ITERATOR_1 & end1) const;
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
		snap(normalized.begin(), normalized.end(), out, snapType & ~ST_NORMALIZE, significands);
		return;
	}
	if (snapType & ST_AUTO) {
		std::vector<mpq_class> tmp(dims);
		std::size_t bestGrade = std::numeric_limits<std::size_t>::max();
		int bestType = ST_NONE;
		constexpr std::array<int, ST__INTERNAL_NUMBER_OF_SNAPPING_TYPES> snappingType = {{ST_FL, ST_FX, ST_CF, ST_JP}};
		for(int st : snappingType) {
			if ((st << ST__INTERNAL_NUMBER_OF_SNAPPING_TYPES) & snapType) {
				snapNormalized(begin, end, tmp.begin(), (snapType & ~ST__INTERNAL_AUTO_ALL_WITH_POLICY) | st, significands, dims);
				std::size_t myGrade = std::numeric_limits<std::size_t>::max();
				if (snapType & ST_AUTO_POLICY_MIN_SUM_DENOM) {
					myGrade = summedDenomSize(tmp.cbegin(), tmp.cend());
				}
				else if (snapType & ST_AUTO_POLICY_MIN_MAX_DENOM) {
					myGrade = maxDenom(tmp.cbegin(), tmp.cend());
				}
				else if (snapType & ST_AUTO_POLICY_MIN_SQUARED_DISTANCE) {
					mpq_class sqDistance = squaredDistance(begin, tmp.cbegin(), end);
					myGrade = (Conversion<mpq_class>::toMpreal(sqDistance, 64) << 60).toULong();
				}
				else if (snapType & ST_AUTO_POLICY_MIN_MAX_NORM) {
					mpq_class sqDistance = maxNorm(begin, tmp.cbegin(), end);
					myGrade = (Conversion<mpq_class>::toMpreal(sqDistance, 64) << 60).toULong();
				}
				else { //default to ST_AUTO_POLICY_MIN_TOTAL_LIMBS
					myGrade = limbCount(tmp.cbegin(), tmp.cend());
				}
				if (bestGrade > myGrade) {
					bestGrade = myGrade;
					bestType = st;
				}
			}
		}
		if (bestType == ST_NONE) {
			bestType = ST_FX;
		}
		snapNormalized(begin, end, out, (snapType & ~ST__INTERNAL_AUTO_ALL_WITH_POLICY) | bestType, significands, dims);
	}
	else {
		snapNormalized(begin, end, out, snapType, significands, dims);
	}
}

template<typename T_INPUT_ITERATOR, typename T_OUTPUT_ITERATOR>
void ProjectSN::snap(T_INPUT_ITERATOR begin, T_INPUT_ITERATOR end, T_OUTPUT_ITERATOR out, const SnapConfig & sc) const {
	using std::distance;
	snap(begin, end, out, sc.snapType(), sc.significands(distance(begin, end)));
}

//private implementations
template<typename T_INPUT_ITERATOR, typename T_OUTPUT_ITERATOR>
void ProjectSN::snapNormalized(T_INPUT_ITERATOR begin, T_INPUT_ITERATOR end, T_OUTPUT_ITERATOR out, int snapType, int significands, std::size_t dims) const {
	using input_ft = typename std::iterator_traits<T_INPUT_ITERATOR>::value_type;

	std::vector<mpq_class> coords_plane_pq(dims);
	PositionOnSphere pos;
	if (snapType & ST_SPHERE) {
		std::vector<mpq_class> coords_sphere_pq(dims);
		calc().toRational(begin, end, coords_sphere_pq.begin(), snapType, significands);
		pos = sphere2Plane(coords_sphere_pq.begin(), coords_sphere_pq.end(), coords_plane_pq.begin());
	}
	else if (snapType & ST_PLANE) {
		std::vector<input_ft> coords_plane(dims);
		pos = sphere2Plane(begin, end, coords_plane.begin());
		//this fixes the eps guarantee at the cost of 2 more bits. This is independent of the number of bits
		//The question remains: why?
// 		if (significands > 0 && snapType & (ST_CF|ST_FX)) {
// 			significands += 2;
// 		}
		if (snapType & ST_JP) {
			int skipDim = std::abs(pos);
			using SkipInputIterator = internal::SkipIterator<typename std::vector<input_ft>::const_iterator>;
			using SkipOutputIterator = internal::SkipIterator<std::vector<mpq_class>::iterator>;
			calc().toRational(
				SkipInputIterator(coords_plane.cbegin(), skipDim),
				SkipInputIterator(coords_plane.cend(), 0),
				SkipOutputIterator(coords_plane_pq.begin(), skipDim),
				snapType, significands);
		}
		else {
			calc().toRational(coords_plane.cbegin(), coords_plane.cend(), coords_plane_pq.begin(), snapType, significands);
		}
	}
	else {
		throw std::runtime_error("ratss::ProjectSN::snap: Unsupported snap type");
	}
	plane2Sphere(coords_plane_pq.begin(), coords_plane_pq.end(), pos, out);
}

template<typename T_ITERATOR>
std::size_t ProjectSN::summedDenomSize(T_ITERATOR begin, const T_ITERATOR & end) const {
	std::size_t result = 0;
	for(; begin != end; ++begin) {
		result += mpz_sizeinbase(begin->get_den().get_mpz_t(), 2);
	}
	return result;
}

template<typename T_ITERATOR>
std::size_t ProjectSN::maxDenom(T_ITERATOR begin, const T_ITERATOR & end) const {
	std::size_t result = 0;
	for(; begin != end; ++begin) {
		result = std::max<std::size_t>(result, mpz_sizeinbase(begin->get_den().get_mpz_t(), 2));
	}
	return result;
}

template<typename T_ITERATOR>
std::size_t ProjectSN::limbCount(T_ITERATOR begin, const T_ITERATOR & end) const {
	std::size_t result = 0;
	for(; begin != end; ++begin) {
		result += __GMPXX_BITS_TO_LIMBS(mpz_sizeinbase(begin->get_num().get_mpz_t(), 2));
		result += __GMPXX_BITS_TO_LIMBS(mpz_sizeinbase(begin->get_den().get_mpz_t(), 2));
	}
	return result;
}


template<typename T_ITERATOR_1, typename T_ITERATOR_2>
mpq_class ProjectSN::squaredDistance(T_ITERATOR_1 begin1, T_ITERATOR_2 begin2, const T_ITERATOR_1 & end1) const {
	using value_type1 = typename std::iterator_traits<T_ITERATOR_1>::value_type;
	using value_type2 = typename std::iterator_traits<T_ITERATOR_2>::value_type;
	mpq_class sq = 0;
	for(; begin1 != end1; ++begin1, ++begin2) {
		mpq_class tmp = Conversion<value_type1>::toMpq(*begin1) - Conversion<value_type2>::toMpq(*begin2);
		sq += tmp*tmp;
	}
	return sq;
}

template<typename T_ITERATOR_1, typename T_ITERATOR_2>
mpq_class ProjectSN::maxNorm(T_ITERATOR_1 begin1, T_ITERATOR_2 begin2, const T_ITERATOR_1 & end1) const {
	using value_type1 = typename std::iterator_traits<T_ITERATOR_1>::value_type;
	using value_type2 = typename std::iterator_traits<T_ITERATOR_2>::value_type;
	using std::abs;
	using std::max;
	using std::distance;
	
	if (distance(begin1, end1) == 0) {
		return mpq_class(0);
	}
	
	mpq_class result = abs(Conversion<value_type1>::toMpq(*begin1) - Conversion<value_type2>::toMpq(*begin2));
	for(++begin1, ++begin2; begin1 != end1; ++begin1, ++begin2) {
		mpq_class tmp = abs(Conversion<value_type1>::toMpq(*begin1) - Conversion<value_type2>::toMpq(*begin2));
		result = max(result, tmp);
	}
	return result;
}


} //end namespace LIB_RATSS_NAMESPACE

#endif