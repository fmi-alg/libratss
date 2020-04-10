#ifndef LIB_RATSS_PROJECT_SN_h
#define LIB_RATSS_PROJECT_SN_h
#pragma once

#include <libratss/constants.h>
#include <libratss/Calc.h>

#include "internal/SkipIterator.h"

#include <assert.h>
#include <array>
#include <vector>

namespace LIB_RATSS_NAMESPACE {

class ProjectSN {
public:
	using SnapType = LIB_RATSS_NAMESPACE::SnapType;
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
	///[begin, end) may point to the same storage as out
	template<typename T_FT_INPUT_ITERATOR, typename T_FT_OUTPUT_ITERATOR>
	PositionOnSphere sphere2Plane(T_FT_INPUT_ITERATOR begin, const T_FT_INPUT_ITERATOR & end, T_FT_OUTPUT_ITERATOR out, PositionOnSphere pos = SP_INVALID) const WARN_UNUSED_RESULT;
	
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
	template<typename GRADE_TYPE, int POLICY>
	struct StOptimizer {
		const ProjectSN * parent;
		int snapType;
		int significands;
		std::size_t dims;
		StOptimizer(const ProjectSN * parent, int snapType, int significands, std::size_t dims);
		template<typename T_ITERATOR>
		int best(const T_ITERATOR & begin, const T_ITERATOR & end) const;
		template<typename T_ITERATOR_INPUT, typename T_ITERATOR_OUTPUT>
		GRADE_TYPE grade(const T_ITERATOR_INPUT & input_begin, const T_ITERATOR_INPUT & input_end, const T_ITERATOR_OUTPUT & output_begin, const T_ITERATOR_OUTPUT & output_end) const;
	};
private:
	template<typename T_INPUT_ITERATOR, typename T_OUTPUT_ITERATOR>
	void snapNormalized(T_INPUT_ITERATOR begin, T_INPUT_ITERATOR end, T_OUTPUT_ITERATOR out, int snapType, int significands, std::size_t dims) const;
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
	value_type v( (unsigned int)(0) );
	for(int p(1); begin != end; ++begin, ++p) {
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
PositionOnSphere ProjectSN::sphere2Plane(T_FT_INPUT_ITERATOR begin, const T_FT_INPUT_ITERATOR& end, T_FT_OUTPUT_ITERATOR out, ratss::PositionOnSphere pos) const {
	using std::iterator_traits;
	using FT = typename iterator_traits<T_FT_INPUT_ITERATOR>::value_type;
	if (begin == end) {
		return SP_INVALID;
	}
	if (pos == SP_INVALID) {
		pos = positionOnSphere(begin, end);
	}
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
	
	if (snapType & ST_PAPER) {
#if defined(LIB_RATSS_WITH_CGAL)
		std::vector<CORE::Expr> ptc(dims);
		std::vector<mpq_class> pt_snap_plane(dims);
		
		std::transform(begin, end, ptc.begin(), [](auto x) -> CORE::Expr { return convert<CORE::Expr>(x); });
		
		//normalize input
		{
			CORE::Expr len{0};
			for(auto & x : ptc) {
				len += x*x;
			}
			len = sqrt(len);
			for(auto & x : ptc) {
				x /= len;
			}
		}
		
		auto pos = this->sphere2Plane(ptc.begin(), ptc.end(), ptc.begin());
		
		if (snapType & (ST_JP | ST_FPLLL | ST_CF)) {
			std::vector<mpq_class> apx_plane;
			apx_plane.reserve(dims);
			for(auto const & x : ptc) {
				apx_plane.emplace_back( convert<mpq_class>( x.approx(significands+2, significands+2) ) );
			}
			if (snapType & ST_JP) {
				int skipDim = std::abs(pos);
				using SkipInputIterator = internal::SkipIterator<typename std::vector<mpq_class>::const_iterator>;
				using SkipOutputIterator = internal::SkipIterator<std::vector<mpq_class>::iterator>;
				calc().toRational(
					SkipInputIterator(apx_plane.cbegin(), skipDim),
					SkipInputIterator(apx_plane.cend(), 0),
					SkipOutputIterator(pt_snap_plane.begin(), skipDim),
					snapType, significands);
			}
			else {
				calc().toRational(apx_plane.cbegin(), apx_plane.cend(), pt_snap_plane.begin(), snapType, significands);
			}
		}
		else if (snapType & ST_FX) {
			for(std::size_t i(0); i < dims; ++i) {
				CORE::BigFloat fv = ptc[i].approx(significands+1, significands+1).BigFloatValue();
				fv = calc().toFixpoint(fv, significands);
				pt_snap_plane[i] = Conversion<CORE::BigFloat>::toMpq(fv);
			}
		}
		else {
			throw std::runtime_error("ProjectSN::snap: snapType ST_PAPER is incompatible with ST_FL");
		}

		this->plane2Sphere(pt_snap_plane.begin(), pt_snap_plane.end(), pos, out);
#else
		throw std::runtime_error("libratss was built without CGAL support");
#endif
	}
	else {
		if (snapType & ST_NORMALIZE) {
			std::vector<input_ft> normalized(dims);
			calc().normalize(begin, end, normalized.begin());
			snap(normalized.begin(), normalized.end(), out, snapType & ~ST_NORMALIZE, significands);
			return;
		}
		if (snapType & ST_AUTO) {
			int bestType = ST_FX;
			if (snapType & ST_AUTO_POLICY_MIN_MAX_DENOM) {
				StOptimizer<std::size_t, ST_AUTO_POLICY_MIN_MAX_DENOM> optimizer(this, snapType, significands, dims);
				bestType = optimizer.best(begin, end);
			}
			else if (snapType & ST_AUTO_POLICY_MIN_SUM_DENOM) {
				StOptimizer<std::size_t, ST_AUTO_POLICY_MIN_SUM_DENOM> optimizer(this, snapType, significands, dims);
				bestType = optimizer.best(begin, end);
			}
			else if (snapType & ST_AUTO_POLICY_MIN_TOTAL_LIMBS) {
				StOptimizer<std::size_t, ST_AUTO_POLICY_MIN_TOTAL_LIMBS> optimizer(this, snapType, significands, dims);
				bestType = optimizer.best(begin, end);
			}
			else if (snapType & ST_AUTO_POLICY_MIN_MAX_NORM) {
				StOptimizer<mpq_class, ST_AUTO_POLICY_MIN_MAX_NORM> optimizer(this, snapType, significands, dims);
				bestType = optimizer.best(begin, end);
			}
			else if (snapType & ST_AUTO_POLICY_MIN_SQUARED_DISTANCE) {
				StOptimizer<mpq_class, ST_AUTO_POLICY_MIN_SQUARED_DISTANCE> optimizer(this, snapType, significands, dims);
				bestType = optimizer.best(begin, end);
			}
			else {
				throw std::runtime_error("ratss::ProjectSN::snap: auto snapping requested, but no policy was set");
			}
			snapNormalized(begin, end, out, (snapType & ~ST__INTERNAL_AUTO_ALL_WITH_POLICY) | bestType, significands, dims);
		}
		else {
			snapNormalized(begin, end, out, snapType, significands, dims);
		}
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


template<typename GRADE_TYPE, int POLICY>
ProjectSN::StOptimizer<GRADE_TYPE, POLICY>::StOptimizer(const ProjectSN * _parent, int _snapType, int _significands, std::size_t _dims) :
parent(_parent),
snapType(_snapType),
significands(_significands),
dims(_dims)
{}

template<typename GRADE_TYPE, int POLICY>
template<typename T_ITERATOR>
int
ProjectSN::StOptimizer<GRADE_TYPE, POLICY>::best(const T_ITERATOR & begin, const T_ITERATOR & end) const {
	constexpr std::array<int, ST__INTERNAL_NUMBER_OF_SNAPPING_TYPES> snappingType = {{ST_FL, ST_FX, ST_CF, ST_JP, ST_FPLLL}};
	std::vector<mpq_class> tmp(dims);
	GRADE_TYPE bestGrade = GRADE_TYPE(std::numeric_limits<std::size_t>::max());
	int bestType = ST_FX;
	for(int st : snappingType) {
		if ((st << ST__INTERNAL_NUMBER_OF_SNAPPING_TYPES) & snapType) {
			parent->snapNormalized(begin, end, tmp.begin(), (snapType & ~ST__INTERNAL_AUTO_ALL_WITH_POLICY) | st, significands, dims);
			GRADE_TYPE myGrade = grade(begin, end, tmp.begin(), tmp.end());
			if (bestGrade > myGrade) {
				bestGrade = myGrade;
				bestType = st;
			}
		}
	}
	return bestType;
}

template<>
template<typename T_ITERATOR_INPUT, typename T_ITERATOR_OUTPUT>
std::size_t
ProjectSN::StOptimizer<std::size_t, ProjectSN::SnapType::ST_AUTO_POLICY_MIN_SUM_DENOM>::
grade(const T_ITERATOR_INPUT & /*input_begin*/, const T_ITERATOR_INPUT & /*input_end*/, const T_ITERATOR_OUTPUT & output_begin, const T_ITERATOR_OUTPUT & output_end) const {
	return parent->calc().summedDenomSize(output_begin, output_end);
}

template<>
template<typename T_ITERATOR_INPUT, typename T_ITERATOR_OUTPUT>
std::size_t
ProjectSN::StOptimizer<std::size_t, ProjectSN::SnapType::ST_AUTO_POLICY_MIN_TOTAL_LIMBS>::
grade(const T_ITERATOR_INPUT & /*input_begin*/, const T_ITERATOR_INPUT & /*input_end*/, const T_ITERATOR_OUTPUT & output_begin, const T_ITERATOR_OUTPUT & output_end) const {
	return parent->calc().limbCount(output_begin, output_end);
}

template<>
template<typename T_ITERATOR_INPUT, typename T_ITERATOR_OUTPUT>
std::size_t
ProjectSN::StOptimizer<std::size_t, ProjectSN::SnapType::ST_AUTO_POLICY_MIN_MAX_DENOM>::
grade(const T_ITERATOR_INPUT & /*input_begin*/, const T_ITERATOR_INPUT & /*input_end*/, const T_ITERATOR_OUTPUT & output_begin, const T_ITERATOR_OUTPUT & output_end) const {
	return parent->calc().maxDenom(output_begin, output_end);
}

template<>
template<typename T_ITERATOR_INPUT, typename T_ITERATOR_OUTPUT>
mpq_class
ProjectSN::StOptimizer<mpq_class, ProjectSN::SnapType::ST_AUTO_POLICY_MIN_SQUARED_DISTANCE>::
grade(const T_ITERATOR_INPUT & input_begin, const T_ITERATOR_INPUT & input_end, const T_ITERATOR_OUTPUT & output_begin, const T_ITERATOR_OUTPUT & /*output_end*/) const {
	return parent->calc().squaredDistance(input_begin, input_end, output_begin);
}

template<>
template<typename T_ITERATOR_INPUT, typename T_ITERATOR_OUTPUT>
mpq_class
ProjectSN::StOptimizer<mpq_class, ProjectSN::SnapType::ST_AUTO_POLICY_MIN_MAX_NORM>::
grade(const T_ITERATOR_INPUT & input_begin, const T_ITERATOR_INPUT & input_end, const T_ITERATOR_OUTPUT & output_begin, const T_ITERATOR_OUTPUT & /*output_end*/) const {
	return parent->calc().maxNorm(input_begin, input_end, output_begin);
}

} //end namespace LIB_RATSS_NAMESPACE

#endif
