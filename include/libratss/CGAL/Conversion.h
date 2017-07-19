#ifndef LIB_RATSS_CGAL_CONVERSION_H
#define LIB_RATSS_CGAL_CONVERSION_H
#pragma once

#ifndef LIB_RATSS_CONVERSION_H
	#warning "libratss/CGAL/Conversion.h should not be included directly"
#endif

#include <CGAL/CORE/BigRat.h>
#include <CGAL/CORE/Expr.h>
#include <CGAL/Gmpq.h>
#include <CGAL/Lazy_exact_nt.h>
#include <libratss/CGAL/ExtendedInt64Pq.h>

namespace LIB_RATSS_NAMESPACE {

template<>
struct Conversion< CGAL::ExtendedInt64Pq<CGAL::Gmpq> > {
	using type = CGAL::ExtendedInt64Pq<CGAL::Gmpq>;
	static type moveFrom(const mpq_class & v);
	static mpq_class toMpq(const type & v);
	static mpfr::mpreal toMpreal(const type & v, int precision);
};

template<>
struct Conversion<
	CGAL::Lazy_exact_nt<
		CGAL::ExtendedInt64Pq<CGAL::Gmpq>
	>
>
{
	using type = CGAL::Lazy_exact_nt< CGAL::ExtendedInt64Pq<CGAL::Gmpq> >;
	static type moveFrom(const mpq_class & v);
	static mpq_class toMpq(const type & v);
	static mpfr::mpreal toMpreal(const type & v, int precision);
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

}//end namespace LIB_RATSS_NAMESPACE

#endif