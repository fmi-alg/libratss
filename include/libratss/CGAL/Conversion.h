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
#include <gmpxx.h>
#include <boost/multiprecision/gmp.hpp>

namespace LIB_RATSS_NAMESPACE {
	
template<typename EFT>
struct Conversion<
	CGAL::Lazy_exact_nt<EFT>
>
{
	using type = CGAL::Lazy_exact_nt<EFT>;
	static type moveFrom(const mpq_class & v) {
		return type( Conversion<EFT>::moveFrom(v) );
	}
	static mpq_class toMpq(const type & v) {
		return Conversion<EFT>::toMpq( v.exact() );
	}
	static mpfr::mpreal toMpreal(const type & v, int precision) {
		return Conversion<EFT>::toMpreal(v.exact(), precision);
	}
};

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
struct Conversion< CGAL::ExtendedInt64q<CGAL::Gmpq> > {
	using type = CGAL::ExtendedInt64q<CGAL::Gmpq>;
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
struct Conversion< boost::multiprecision::number<boost::multiprecision::backends::gmp_rational> > {
	using type = boost::multiprecision::number<boost::multiprecision::backends::gmp_rational>;
	static type moveFrom(const mpq_class & v);
	static mpq_class toMpq(const type & v);
	static mpfr::mpreal toMpreal(const type & v, int precision);
};

template<>
struct Conversion<CORE::BigInt> {
	using type = CORE::BigInt;
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

template<>
struct Conversion<CORE::BigFloat> {
	using type = CORE::BigFloat;
	static mpq_class toMpq(const type & v);
	static mpfr::mpreal toMpreal(const type & v, int precision);
};

template<>
struct Conversion<CORE::Expr> {
	using type = CORE::Expr;
	static type moveFrom(const mpq_class & v);
	static mpq_class toMpq(const type & v, int precision = -1);
	static mpfr::mpreal toMpreal(const type & v, int precision);
};

template<>
struct Conversion<CORE::Real> {
	using type = CORE::Real;
	static type moveFrom(const mpq_class & v);
	static mpq_class toMpq(const type & v, int precision = -1);
	static mpfr::mpreal toMpreal(const type & v, int precision);
};

#if CGAL_VERSION_NR >= 1041101000 && defined(CGAL_USE_GMPXX)

template<>
struct Conversion< CGAL::Lazy_exact_nt<mpq_class> > {
	using type = CGAL::Lazy_exact_nt<mpq_class>;
	static type moveFrom(mpq_class const & v);
	static type moveFrom(mpq_class & v);
	static type moveFrom(mpq_class && v);
	static mpq_class toMpq(const type & v);
	static mpfr::mpreal toMpreal(const type & v, int precision);
};

#endif

}//end namespace LIB_RATSS_NAMESPACE

#endif
