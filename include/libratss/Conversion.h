#ifndef LIB_RATSS_CONVERSION_H
#define LIB_RATSS_CONVERSION_H
#pragma once

#include <libratss/constants.h>
#include <libratss/Precision.h>

#include <gmpxx.h>
#include <limits>
#include <libratss/mpreal.h>
#include <CGAL/CORE/BigRat.h>
#include <CGAL/CORE/Expr.h>
#include <CGAL/Gmpq.h>
#include <CGAL/Lazy_exact_nt.h>

namespace LIB_RATSS_NAMESPACE {

namespace internal {

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
} //end namespace internal

}//end namespace LIB_DTS2_NAMESPACE

//definitions
namespace LIB_RATSS_NAMESPACE {
namespace internal {


template<>
struct Conversion<mpq_class> {
	using type = mpq_class;
	static type && moveFrom(mpq_class && v);
	static const mpq_class & toMpq(const type & v);
	static mpfr::mpreal toMpreal(const type & v, int precision);
};

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
struct Conversion<CGAL::Gmpq> {
	using type = CGAL::Gmpq;
	static type moveFrom(const mpq_class & v);
	static mpq_class toMpq(const type & v);
	static mpfr::mpreal toMpreal(const type & v, int precision);
};

template<>
struct Conversion< CGAL::Lazy_exact_nt<CGAL::Gmpq> > {
	using type = CGAL::Lazy_exact_nt<CGAL::Gmpq>;
	static type moveFrom(const mpq_class & v);
	static mpq_class toMpq(const type & v);
	static mpfr::mpreal toMpreal(const type & v, int precision);
};

template<>
struct Conversion<CORE::Expr> {
	using type = CORE::Expr;
	static type moveFrom(const mpq_class & v);
	static mpq_class toMpq(const type & v);
	static mpfr::mpreal toMpreal(const type & v, int precision);
};

template<>
struct Conversion<CORE::BigRat> {
	using type = CORE::BigRat;
	static type moveFrom(const mpq_class & v);
	static mpq_class toMpq(const type & v);
	static mpfr::mpreal toMpreal(const type & v, int precision);
};

}}//end namespace LIB_RATSS_NAMESPACE::internal

#endif