#ifndef LIB_RATSS_PROJECT_SN_h
#define LIB_RATSS_PROJECT_SN_h
#pragma once

#include <libratss/constants.h>
#include <libratss/Calc.h>
#include <libratss/enum.h>

namespace LIB_RATSS_NAMESPACE {

class ProjectSN {
public:
	template<typename T_FT_ITERATOR>
	PositionOnSphere positionOnSphere(T_FT_ITERATOR begin, const T_FT_ITERATOR & end) const;
	
	///Projects the coordinates of begin->end onto new coordinates such that one coordinate is zero
	///If you want to reproject onto the sphere, then you need to store the return value
	template<typename T_FT_INPUT_ITERATOR, typename T_FT_OUTPUT_ITERATOR>
	PositionOnSphere stProject(T_FT_INPUT_ITERATOR begin, const T_FT_INPUT_ITERATOR & end, T_FT_OUTPUT_ITERATOR out) const;
	
	template<typename T_FT_INPUT_ITERATOR, typename T_FT_OUTPUT_ITERATOR>
	void stInverseProject(T_FT_INPUT_ITERATOR begin, const T_FT_INPUT_ITERATOR & end, PositionOnSphere pos, T_FT_OUTPUT_ITERATOR out) const;
public:
	inline const Calc & calc() const { return m_calc; }
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
PositionOnSphere ProjectSN::stProject(T_FT_INPUT_ITERATOR begin, const T_FT_INPUT_ITERATOR & end, T_FT_OUTPUT_ITERATOR out) const {
	using std::iterator_traits;
	using FT = typename iterator_traits<T_FT_INPUT_ITERATOR>::value_type;
	PositionOnSphere pos = positionOnSphere(begin, end);
	int projCoord = abs((int) pos); //starts from 1
	//first get the value of our projection coordinate
	FT projVal = *std::next(begin, projCoord-1);
	FT denom;
	if (pos < 0) {
		denom = (1-projVal);
	}
	else {
		denom = (1+projVal);
	}
	
	T_FT_INPUT_ITERATOR it(begin);
	for(int i(1); i < projCoord; ++i, ++it, ++out) {
		*out = *it / denom;
	}
	//now comes the projection coordinate
	*out = 0;
	++out;
	++it;
	//and the rest
	for( ; it != end; ++it, ++out) {
		*out = *it / denom;
	}
	
	return pos;
}

template<typename T_FT_INPUT_ITERATOR, typename T_FT_OUTPUT_ITERATOR>
void ProjectSN::stInverseProject(T_FT_INPUT_ITERATOR begin, const T_FT_INPUT_ITERATOR & end, PositionOnSphere pos, T_FT_OUTPUT_ITERATOR out) const {
	using std::iterator_traits;
	using std::distance;
	using FT = typename iterator_traits<T_FT_INPUT_ITERATOR>::value_type;
	FT denom(1);
	int projCoord = abs((int) pos); //starts from 1
	assert(projCoord <= distance(begin, end));
	{
		T_FT_INPUT_ITERATOR it(begin);
		for(int i(1); i < projCoord; ++i, ++it) {
			denom += (*it) * (*it);
		}
		//now comes the projection coordinate, skip it
		++it;
		//and the rest
		for( ; it != end; ++it) {
			denom += (*it) * (*it);
		}
	}
	{
		T_FT_INPUT_ITERATOR it(begin);
		for(int i(1); i < projCoord; ++i, ++it, ++out) {
			*out = (2 * (*it) ) / denom;
		}
		//and the projection coordinate
		*out = (std::signbit<int>(pos) ? 1 : -1) * (denom - 2) / denom;
		++out;
		++it;
		//and the rest
		for( ; it != end; ++it, ++out) {
			*out = (2 * (*it) ) / denom;
		}
	}
}

} //end namespace LIB_RATSS_NAMESPACE

#endif