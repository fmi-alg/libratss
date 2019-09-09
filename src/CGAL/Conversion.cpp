#include <libratss/Conversion.h>
#include <libratss/CGAL/Conversion.h>
#include <libratss/types.h>

namespace LIB_RATSS_NAMESPACE {

//BEGIN CGAL::internal::boost_int1024q specilizations

Conversion<CGAL::internal::boost_int1024q>::type
Conversion<CGAL::internal::boost_int1024q>::moveFrom(const mpq_class & v) {
	return type( v.get_str() );
}

mpq_class
Conversion<CGAL::internal::boost_int1024q>::toMpq(const type & v) {
	return mpq_class( v.str() );
}

mpfr::mpreal
Conversion<CGAL::internal::boost_int1024q>::toMpreal(const type & v, int precision) {
	return Conversion<mpq_class>::toMpreal( toMpq(v), precision );
}

//END CGAL::internal::boost_int1024q specilizations
//BEGIN CGAL::ExtendedInt64q<CGAL::internal::boost_int1024q> specilizations

Conversion<CGAL::ExtendedInt64q<CGAL::internal::boost_int1024q>>::type
Conversion<CGAL::ExtendedInt64q<CGAL::internal::boost_int1024q>>::moveFrom(const mpq_class & v) {
	return CGAL::ExtendedInt64q<CGAL::internal::boost_int1024q>(
		Conversion<CGAL::ExtendedInt64q<CGAL::internal::boost_int1024q>::extension_type>::moveFrom(v)
	);
}

mpq_class
Conversion< CGAL::ExtendedInt64q<CGAL::internal::boost_int1024q> >::toMpq(const type & v) {
	if (v.isExtended()) {
		return Conversion<CGAL::ExtendedInt64q<CGAL::internal::boost_int1024q>::extension_type>::toMpq( v.getExtended() );
	}
	else {
		return mpq_class(
			gmp_int64_t(v.numerator().get()), 
			gmp_uint64_t(v.denominator().get())
		);
	}
}

mpfr::mpreal
Conversion< CGAL::ExtendedInt64q<CGAL::internal::boost_int1024q> >::toMpreal(const type & v, int precision) {
	return Conversion<CGAL::ExtendedInt64q<CGAL::internal::boost_int1024q>::extension_type>::toMpreal(v.asExtended(), precision);
}

//END CGAL::ExtendedInt64q<CGAL::internal::boost_int1024q> specilizations
//BEGIN CGAL::ExtendedInt64q<CGAL::Gmpq> specilizations

Conversion<CGAL::ExtendedInt64q<CGAL::Gmpq>>::type
Conversion<CGAL::ExtendedInt64q<CGAL::Gmpq>>::moveFrom(const mpq_class & v) {
	return CGAL::ExtendedInt64q<CGAL::Gmpq>( Conversion<CGAL::ExtendedInt64q<CGAL::Gmpq>::extension_type>::moveFrom(v) );
}

mpq_class
Conversion< CGAL::ExtendedInt64q<CGAL::Gmpq> >::toMpq(const type & v) {
	if (v.isExtended()) {
		return Conversion<CGAL::ExtendedInt64q<CGAL::Gmpq>::extension_type>::toMpq( v.getExtended() );
	}
	else {
		return mpq_class(
			gmp_int64_t(v.numerator().get()),
			gmp_uint64_t(v.denominator().get())
		);
	}
}

mpfr::mpreal
Conversion< CGAL::ExtendedInt64q<CGAL::Gmpq> >::toMpreal(const type & v, int precision) {
	return Conversion<CGAL::ExtendedInt64q<CGAL::Gmpq>::extension_type>::toMpreal(v.asExtended(), precision);
}

//END CGAL::ExtendedInt64q<CGAL::Gmpq> specilizations
//BEGIN CGAL::Gmpq specilizations

Conversion<CGAL::Gmpq>::type
Conversion<CGAL::Gmpq>::moveFrom(const mpq_class & v) {
	return CGAL::Gmpq(v.get_mpq_t());
}

mpq_class
Conversion<CGAL::Gmpq>::toMpq(const type & v) {
	return mpq_class(v.mpq());
}

mpfr::mpreal
Conversion<CGAL::Gmpq>::toMpreal(const type & v, int precision) {
	return mpfr::mpreal(v.mpq(), precision);
}

//END CGAL::Gmpq specilizations

Conversion<CORE::Expr>::type
Conversion<CORE::Expr>::moveFrom(const mpq_class & v) {
	return CORE::Expr( CORE::BigRat(v.get_mpq_t()) );
}

mpq_class
Conversion<CORE::Expr>::toMpq(const type & v) {
	return mpq_class(v.BigRatValue().get_mp());
}

mpfr::mpreal
Conversion<CORE::Expr>::toMpreal(const type & v, int precision) {
	return mpfr::mpreal(v.BigRatValue().get_mp(), precision);
}

Conversion<CORE::BigRat>::type
Conversion<CORE::BigRat>::moveFrom(const mpq_class &v) {
	return CORE::BigRat(v.get_mpq_t());
}

mpq_class
Conversion<CORE::BigRat>::toMpq(const type & v) {
	return mpq_class(v.get_mp());
}

mpfr::mpreal
Conversion<CORE::BigRat>::toMpreal(const type & v, int precision) {
	return mpfr::mpreal(v.get_mp(), precision);
}


Conversion<CORE::Real>::type
Conversion<CORE::Real>::moveFrom(const mpq_class &v) {
	return type( Conversion<CORE::BigRat>::moveFrom(v) );
}

mpq_class
Conversion<CORE::Real>::toMpq(const type & v) {
	return mpq_class( Conversion<CORE::BigRat>::toMpq(v.BigRatValue()) );
}

mpfr::mpreal
Conversion<CORE::Real>::toMpreal(const type & v, int precision) {
	return Conversion<CORE::BigRat>::toMpreal(v.BigRatValue(), precision);
}

//BEGIN CGAL::Lazy_exact_nt<mpq_class>

#if CGAL_VERSION_NR >= 1041101000 && defined(CGAL_USE_GMPXX)

Conversion< CGAL::Lazy_exact_nt<mpq_class> >::type
Conversion< CGAL::Lazy_exact_nt<mpq_class> >::moveFrom(mpq_class & v) {
	return type( std::move(v) );
}

Conversion< CGAL::Lazy_exact_nt<mpq_class> >::type
Conversion< CGAL::Lazy_exact_nt<mpq_class> >::moveFrom(mpq_class && v) {
	return type( std::move(v) );
}

mpq_class
Conversion< CGAL::Lazy_exact_nt<mpq_class> >::toMpq(const type & v) {
	return v.exact();
}

mpfr::mpreal
Conversion< CGAL::Lazy_exact_nt<mpq_class> >::toMpreal(const type & v, int precision) {
	return Conversion<mpq_class>::toMpreal(toMpq(v), precision);
}

#endif

//END CGAL::Lazy_exact_nt<mpq_class>

}
