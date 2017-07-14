#ifndef LIB_RATSS_CONVERSION_H
#define LIB_RATSS_CONVERSION_H
#pragma once

#include <libratss/constants.h>
#include <libratss/Precision.h>

#include <gmpxx.h>
#include <libratss/mpreal.h>
#include <limits>

namespace LIB_RATSS_NAMESPACE {

template<typename T_FT>
struct Conversion {
	using type = T_FT;
	//this shall be a move in case T_FT == mpq_class
	static type moveFrom(mpq_class && v);
	
	//this shall return the reference to v in case T_FT == mpq_class
	static mpq_class toMpq(const type & v);
	
	//precision may be ignore if T_FT == mpfr::mpreal
	static mpfr::mpreal toMpreal(const type & v, int precision);
};

}//end namespace LIB_DTS2_NAMESPACE

//definitions
namespace LIB_RATSS_NAMESPACE {

template<>
struct Conversion<double> {
	using type = double;
	static type moveFrom(const mpq_class & v);
	static mpq_class toMpq(const type & v);
	static mpfr::mpreal toMpreal(const type & v, int precision);
};

template<>
struct Conversion<mpfr::mpreal> {
	using type = mpfr::mpreal;
	static type moveFrom(const mpq_class & v);
	static mpq_class toMpq(const type & v);
	static const mpfr::mpreal & toMpreal(const type & v, int precision);
};

template<>
struct Conversion<mpq_class> {
	using type = mpq_class;
	static type && moveFrom(mpq_class && v);
	static const mpq_class & toMpq(const type & v);
	static mpfr::mpreal toMpreal(const type & v, int precision);
};

}//end namespace LIB_RATSS_NAMESPACE

#if defined(LIB_RATSS_WITH_CGAL)
	#include <libratss/CGAL/Conversion.h>
#endif

namespace LIB_RATSS_NAMESPACE {

template<typename T_TARGET, typename T_SOURCE>
T_TARGET convert(const T_SOURCE & v) {
	return Conversion<T_TARGET>::moveFrom( Conversion<T_SOURCE>::toMpq(v) );
}

template<typename T_TARGET>
T_TARGET convert(const mpq_class & v) {
	return Conversion<T_TARGET>::moveFrom( v );
}

}

#endif