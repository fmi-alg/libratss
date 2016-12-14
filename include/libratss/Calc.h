#ifndef LIB_RATSS_CALC_H
#define LIB_RATSS_CALC_H
#pragma once

#include <libratss/constants.h>
#include <libratss/Conversion.h>

namespace LIB_RATSS_NAMESPACE {

class Calc {
public:
	///Values are compatible with the ones defined in ProjectSN::SnapType
	typedef enum { ST_CF=0x4, ST_FX=0x8, ST_FL=0x10 } SnapType;
public:
	mpfr::mpreal sin(const mpfr::mpreal & v) const;
	mpfr::mpreal asin(const mpfr::mpreal & v) const;

	mpfr::mpreal cos(const mpfr::mpreal & v) const;
	mpfr::mpreal acos(const mpfr::mpreal & v) const;
	
	mpfr::mpreal tan(const mpfr::mpreal & v) const;
	mpfr::mpreal atan(const mpfr::mpreal & v) const;
	
	mpfr::mpreal squaredDistance(const mpfr::mpreal & a, const mpfr::mpreal & b) const;
	mpfr::mpreal squaredDistance(const mpfr::mpreal & a, const mpfr::mpreal & b, const mpfr::mpreal & c) const;
	mpfr::mpreal mult(const mpfr::mpreal & a, const mpfr::mpreal & b) const;
	mpfr::mpreal div(const mpfr::mpreal & a, const mpfr::mpreal & b) const;
	mpfr::mpreal sq(const mpfr::mpreal & v) const;
	mpfr::mpreal sqrt(const mpfr::mpreal & v) const;
	mpfr::mpreal add(const mpfr::mpreal & a, const mpfr::mpreal & b)  const;
	mpfr::mpreal sub(const mpfr::mpreal & a, const mpfr::mpreal & b)  const;
	mpfr::mpreal toFixpoint(const mpfr::mpreal & v) const;
	void makeFixpoint(mpfr::mpreal& v) const;
public:
	template<typename T_INPUT_ITERATOR>
	mpfr::mpreal squaredLength(T_INPUT_ITERATOR begin, T_INPUT_ITERATOR end) const;
	///input and output may point to the same storage
	template<typename T_INPUT_ITERATOR, typename T_OUTPUT_ITERATOR>
	void normalize(T_INPUT_ITERATOR begin, T_INPUT_ITERATOR end, T_OUTPUT_ITERATOR out) const;
public:
	///@return r a number satisfying the following conditions:
	/// r is a fraction with the smallest denominator such that lower <= r <= upper
	mpq_class within(const mpq_class& lower, const mpq_class& upper) const;
	
	mpq_class snap(const mpfr::mpreal & v, int st) const;
public:
	template<typename T_INPUT_ITERATOR, typename T_OUTPUT_ITERATOR>
	typename std::enable_if<
		std::is_same<
			typename std::decay<
				typename std::iterator_traits<T_INPUT_ITERATOR>::value_type
			>::type,
			mpfr::mpreal
		>::value,
		void
	>::type
	toRational(T_INPUT_ITERATOR begin, T_INPUT_ITERATOR end, T_OUTPUT_ITERATOR out, int snapType) const;
	
	template<typename T_INPUT_ITERATOR, typename T_OUTPUT_ITERATOR>
	typename std::enable_if<
		std::is_same<
			typename std::decay<
				typename std::iterator_traits<T_INPUT_ITERATOR>::value_type
			>::type,
			mpq_class
		>::value,
		void
	>::type
	toRational(T_INPUT_ITERATOR begin, T_INPUT_ITERATOR end, T_OUTPUT_ITERATOR out, int snapType) const;
public:
	std::size_t maxBitCount(const mpq_class &v) const;
};

}//end namespace LIB_RATSS_NAMESPACE

//definitions

namespace LIB_RATSS_NAMESPACE {

template<typename T_INPUT_ITERATOR>
mpfr::mpreal Calc::squaredLength(T_INPUT_ITERATOR begin, T_INPUT_ITERATOR end) const {
	mpfr::mpreal tmp(0);
	for(T_INPUT_ITERATOR it(begin); it != end; ++it) {
		tmp = add(sq(*begin), tmp);
	}
	return tmp;
}

template<typename T_INPUT_ITERATOR, typename T_OUTPUT_ITERATOR>
void Calc::normalize(T_INPUT_ITERATOR begin, T_INPUT_ITERATOR end, T_OUTPUT_ITERATOR out) const {
	mpfr::mpreal tmp = sqrt( squaredLength(begin, end) );
	for(; begin != end; ++begin, ++out) {
		*out = div(*begin, tmp);
	}
}


template<typename T_INPUT_ITERATOR, typename T_OUTPUT_ITERATOR>
typename std::enable_if<
	std::is_same<
		typename std::decay<
			typename std::iterator_traits<T_INPUT_ITERATOR>::value_type
		>::type,
		mpq_class
	>::value,
	void
>::type
Calc::toRational(T_INPUT_ITERATOR begin, T_INPUT_ITERATOR end, T_OUTPUT_ITERATOR out, int snapType) const {
	for( ;begin != end; ++begin, ++out) {
		*out = *begin;
	}
}

template<typename T_INPUT_ITERATOR, typename T_OUTPUT_ITERATOR>
typename std::enable_if<
	std::is_same<
		typename std::decay<
			typename std::iterator_traits<T_INPUT_ITERATOR>::value_type
		>::type,
		mpfr::mpreal
	>::value,
	void
>::type
Calc::toRational(T_INPUT_ITERATOR begin, T_INPUT_ITERATOR end, T_OUTPUT_ITERATOR out, int snapType) const {
	std::transform(
		begin,
		end,
		out,
		[this, snapType](const mpfr::mpreal & v) {
			return snap(v, snapType);
		}
	);
}

}//end namespace LIB_RATSS_NAMESPACE

#endif