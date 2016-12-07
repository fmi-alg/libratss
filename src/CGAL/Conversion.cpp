#include <libratss/Conversion.h>

namespace LIB_RATSS_NAMESPACE {

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