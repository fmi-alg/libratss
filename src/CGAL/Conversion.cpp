#include <libratss/Conversion.h>
#include <libratss/CGAL/Conversion.h>

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
		return mpq_class(v.numerator().get(), v.denominator().get());
	}
}

mpfr::mpreal
Conversion< CGAL::ExtendedInt64q<CGAL::internal::boost_int1024q> >::toMpreal(const type & v, int precision) {
	return Conversion<CGAL::ExtendedInt64q<CGAL::internal::boost_int1024q>::extension_type>::toMpreal(v.asExtended(), precision);
}

Conversion< CGAL::Lazy_exact_nt< CGAL::ExtendedInt64q<CGAL::internal::boost_int1024q> > >::type
Conversion< CGAL::Lazy_exact_nt< CGAL::ExtendedInt64q<CGAL::internal::boost_int1024q> > >::moveFrom(const mpq_class & v) {
	return type( Conversion< CGAL::ExtendedInt64q<CGAL::internal::boost_int1024q> >::moveFrom(v) );
}

mpq_class
Conversion< CGAL::Lazy_exact_nt< CGAL::ExtendedInt64q<CGAL::internal::boost_int1024q> > >::toMpq(const type & v) {
	return Conversion< CGAL::ExtendedInt64q<CGAL::internal::boost_int1024q> >::toMpq(v.exact());
}

mpfr::mpreal
Conversion< CGAL::Lazy_exact_nt< CGAL::ExtendedInt64q<CGAL::internal::boost_int1024q> > >::toMpreal(const type & v, int precision) {
	return Conversion< CGAL::ExtendedInt64q<CGAL::internal::boost_int1024q> >::toMpreal(v.exact(), precision);
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
		return mpq_class(v.numerator().get(), v.denominator().get());
	}
}

mpfr::mpreal
Conversion< CGAL::ExtendedInt64q<CGAL::Gmpq> >::toMpreal(const type & v, int precision) {
	return Conversion<CGAL::ExtendedInt64q<CGAL::Gmpq>::extension_type>::toMpreal(v.asExtended(), precision);
}

Conversion< CGAL::Lazy_exact_nt< CGAL::ExtendedInt64q<CGAL::Gmpq> > >::type
Conversion< CGAL::Lazy_exact_nt< CGAL::ExtendedInt64q<CGAL::Gmpq> > >::moveFrom(const mpq_class & v) {
	return type( Conversion< CGAL::ExtendedInt64q<CGAL::Gmpq> >::moveFrom(v) );
}

mpq_class
Conversion< CGAL::Lazy_exact_nt< CGAL::ExtendedInt64q<CGAL::Gmpq> > >::toMpq(const type & v) {
	return Conversion< CGAL::ExtendedInt64q<CGAL::Gmpq> >::toMpq(v.exact());
}

mpfr::mpreal
Conversion< CGAL::Lazy_exact_nt< CGAL::ExtendedInt64q<CGAL::Gmpq> > >::toMpreal(const type & v, int precision) {
	return Conversion< CGAL::ExtendedInt64q<CGAL::Gmpq> >::toMpreal(v.exact(), precision);
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

Conversion< CGAL::Lazy_exact_nt<CGAL::Gmpq> >::type
Conversion< CGAL::Lazy_exact_nt<CGAL::Gmpq> >::moveFrom(const mpq_class & v) {
	return type(Conversion<CGAL::Gmpq>::moveFrom(v));
}

mpq_class
Conversion< CGAL::Lazy_exact_nt<CGAL::Gmpq> >::toMpq(const type & v) {
	return Conversion<CGAL::Gmpq>::toMpq(v.exact());
}

mpfr::mpreal
Conversion< CGAL::Lazy_exact_nt<CGAL::Gmpq> >::toMpreal(const type & v, int precision) {
	return Conversion<CGAL::Gmpq>::toMpreal(v.exact(), precision);
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

}