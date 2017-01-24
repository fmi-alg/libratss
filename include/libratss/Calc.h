#ifndef LIB_RATSS_CALC_H
#define LIB_RATSS_CALC_H
#pragma once

#include <libratss/constants.h>
#include <libratss/Conversion.h>

namespace LIB_RATSS_NAMESPACE {

class Calc {
public:
	///Values are compatible with the ones defined in ProjectSN::SnapType
	typedef enum { ST_NONE=0x0, ST_CF=0x4, ST_FX=0x8, ST_FL=0x10, ST_JP=0x20 } SnapType;
public:
	template<typename T_FT>
	inline T_FT add(const T_FT & a, const T_FT & b) const { return a+b; }

	template<typename T_FT>
	inline T_FT sub(const T_FT & a, const T_FT & b) const { return a-b; }

	template<typename T_FT>
	inline T_FT mult(const T_FT & a, const T_FT & b) const { return a*b; }
	
	template<typename T_FT>
	inline T_FT div(const T_FT & a, const T_FT & b) const { return a/b; }
public:
	mpfr::mpreal add(const mpfr::mpreal & a, const mpfr::mpreal & b)  const;
	mpfr::mpreal sub(const mpfr::mpreal & a, const mpfr::mpreal & b)  const;
	mpfr::mpreal mult(const mpfr::mpreal & a, const mpfr::mpreal & b) const;
	mpfr::mpreal div(mpfr::mpreal a, const mpfr::mpreal& b) const;
public:
	mpfr::mpreal sin(const mpfr::mpreal & v) const;
	mpfr::mpreal asin(const mpfr::mpreal & v) const;

	mpfr::mpreal cos(const mpfr::mpreal & v) const;
	mpfr::mpreal acos(const mpfr::mpreal & v) const;
	
	mpfr::mpreal tan(const mpfr::mpreal & v) const;
	mpfr::mpreal atan(const mpfr::mpreal & v) const;
	
	mpfr::mpreal squaredDistance(const mpfr::mpreal & a, const mpfr::mpreal & b) const;
	mpfr::mpreal squaredDistance(const mpfr::mpreal & a, const mpfr::mpreal & b, const mpfr::mpreal & c) const;
	mpfr::mpreal sq(const mpfr::mpreal & v) const;
	mpfr::mpreal sqrt(const mpfr::mpreal & v) const;
public:
	mpfr::mpreal toFixpoint(const mpfr::mpreal & v, int significands = -1) const;
	void makeFixpoint(mpfr::mpreal& v, int significands = -1) const;
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
	
	mpq_class contFrac(const mpq_class& value, int significands) const;
	
	void jacobiPerron2D(const mpq_class& input1, const mpq_class& input2, mpq_class& output1, mpq_class& output2, int significands) const;
	
	mpq_class snap(const mpfr::mpreal & v, int st, int eps = -1) const;
public:
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
	toRational(T_INPUT_ITERATOR begin, T_INPUT_ITERATOR end, T_OUTPUT_ITERATOR out, int snapType, int eps = -1) const;
	
	template<typename T_INPUT_ITERATOR, typename T_OUTPUT_ITERATOR>
	typename std::enable_if<
		!std::is_same<
			typename std::decay<
				typename std::iterator_traits<T_INPUT_ITERATOR>::value_type
			>::type,
			mpq_class
		>::value,
		void
	>::type
	toRational(T_INPUT_ITERATOR begin, T_INPUT_ITERATOR end, T_OUTPUT_ITERATOR out, int snapType, int eps = -1) const;
public:
	std::size_t maxBitCount(const mpq_class &v) const;
};

}//end namespace LIB_RATSS_NAMESPACE

//definitions

namespace LIB_RATSS_NAMESPACE {

template<typename T_INPUT_ITERATOR>
mpfr::mpreal Calc::squaredLength(T_INPUT_ITERATOR begin, T_INPUT_ITERATOR end) const {
	mpfr::mpreal tmp(0);
	for(; begin != end; ++begin) {
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
Calc::toRational(T_INPUT_ITERATOR begin, T_INPUT_ITERATOR end, T_OUTPUT_ITERATOR out, int /*snapType*/, int eps) const {
	if (eps < 0) {
		for( ;begin != end; ++begin, ++out) {
			*out = *begin;
		}
	}
	else {
		throw std::runtime_error("Unsupported options: toRational with rational and eps");
	}
}

template<typename T_INPUT_ITERATOR, typename T_OUTPUT_ITERATOR>
typename std::enable_if<
	!std::is_same<
		typename std::decay<
			typename std::iterator_traits<T_INPUT_ITERATOR>::value_type
		>::type,
		mpq_class
	>::value,
	void
>::type
Calc::toRational(T_INPUT_ITERATOR begin, T_INPUT_ITERATOR end, T_OUTPUT_ITERATOR out, int snapType, int significands) const {
	using input_ft = typename std::iterator_traits<T_INPUT_ITERATOR>::value_type;
	if (snapType & ST_JP) {
		using std::distance;
		if (distance(begin, end) != 2) {
			throw std::domain_error("Calc::toRational: Snapping with jacobiPerron only supports dimension 2");
		}
		mpq_class input1( Conversion<input_ft>::toMpq(*begin) );
		++begin;
		mpq_class input2( Conversion<input_ft>::toMpq(*begin) );
		mpq_class output1, output2;
		jacobiPerron2D(input1, input2, output1, output2, significands);
		*out = output1;
		++out;
		*out = output2;
		++out;
	}
	else {
		std::transform(
			begin,
			end,
			out,
			[this, snapType, significands](const input_ft & v) {
				return snap(v, snapType, significands);
			}
		);
	}
}

}//end namespace LIB_RATSS_NAMESPACE

#endif