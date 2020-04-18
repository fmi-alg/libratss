#ifndef LIB_RATSS_CORE2_CONVERSION_H
#define LIB_RATSS_CORE2_CONVERSION_H
#pragma once

#ifndef LIB_RATSS_CONVERSION_H
	#warning "libratss/core2/Conversion.h should not be included directly"
#endif

#include <CORE_TWO.h>

namespace LIB_RATSS_NAMESPACE {

template<>
struct Conversion<CORE_TWO::BigInt> {
	using type = CORE_TWO::BigInt;
	static mpq_class toMpq(const type & v);
	static mpfr::mpreal toMpreal(const type & v, int precision);
};

template<>
struct Conversion<CORE_TWO::BigRat> {
	using type = CORE_TWO::BigRat;
	static type moveFrom(const mpq_class & v);
	static mpq_class toMpq(const type & v);
	static mpfr::mpreal toMpreal(const type & v, int precision);
};

template<>
struct Conversion<CORE_TWO::BigFloat> {
	using type = CORE_TWO::BigFloat;
	static mpq_class toMpq(const type & v);
	static mpfr::mpreal toMpreal(const type & v, int precision);
};

template<>
struct Conversion<CORE_TWO::BigFloat2> {
	using type = CORE_TWO::BigFloat2;
	static mpq_class toMpq(const type & v);
	static mpfr::mpreal toMpreal(const type & v, int precision);
};

template<>
struct Conversion<CORE_TWO::Expr> {
	using type = CORE_TWO::Expr;
	static type moveFrom(const mpq_class & v);
	static mpq_class toMpq(const type & v, int precision = -1);
	static mpfr::mpreal toMpreal(const type & v, int precision);
};

}//end namespace LIB_RATSS_NAMESPACE

#endif
