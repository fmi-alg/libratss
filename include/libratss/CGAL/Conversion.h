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
#include <libratss/CGAL/ExtendedInt64q.h>

namespace LIB_RATSS_NAMESPACE {
	
//make sure that GMP is able to handle 64 Bit numbers.
static_assert(sizeof(uint64_t) <= sizeof(unsigned long), "Unable to initialize GMP types from uint64_t");
static_assert(sizeof(int64_t) <= sizeof(signed long), "Unable to initialize GMP types from int64_t");

template<>
struct Conversion< CGAL::internal::boost_int1024q > {
	using type = CGAL::internal::boost_int1024q;
	static type moveFrom(const mpq_class & v);
	static mpq_class toMpq(const type & v);
	static mpfr::mpreal toMpreal(const type & v, int precision);
};

template<>
struct Conversion< CGAL::ExtendedInt64q<CGAL::internal::boost_int1024q> > {
	using type = CGAL::ExtendedInt64q<CGAL::internal::boost_int1024q>;
	static type moveFrom(const mpq_class & v);
	static mpq_class toMpq(const type & v);
	static mpfr::mpreal toMpreal(const type & v, int precision);
};

template<>
struct Conversion<
	CGAL::Lazy_exact_nt<
		CGAL::ExtendedInt64q<CGAL::internal::boost_int1024q>
	>
>
{
	using type = CGAL::Lazy_exact_nt< CGAL::ExtendedInt64q<CGAL::internal::boost_int1024q> >;
	static type moveFrom(const mpq_class & v);
	static mpq_class toMpq(const type & v);
	static mpfr::mpreal toMpreal(const type & v, int precision);
};

template<>
struct Conversion< CGAL::ExtendedInt64q<CGAL::Gmpq> > {
	using type = CGAL::ExtendedInt64q<CGAL::Gmpq>;
	static type moveFrom(const mpq_class & v);
	static mpq_class toMpq(const type & v);
	static mpfr::mpreal toMpreal(const type & v, int precision);
};

template<>
struct Conversion<
	CGAL::Lazy_exact_nt<
		CGAL::ExtendedInt64q<CGAL::Gmpq>
	>
>
{
	using type = CGAL::Lazy_exact_nt< CGAL::ExtendedInt64q<CGAL::Gmpq> >;
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
