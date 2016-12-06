#include <libratss/Conversion.h>
#include <libratss/mpreal.h>

namespace LIB_RATSS_NAMESPACE {

namespace internal {

//BEGIN double specializations

Conversion<double>::type
Conversion<double>::moveFrom(const mpq_class & v) {
	return Conversion<mpfr::mpreal>::moveFrom(v).toDouble();
}

mpq_class
Conversion<double>::toMpq(const type & v) {
	return Conversion<mpfr::mpreal>::toMpq(mpfr::mpreal(v));
}

mpfr::mpreal
Conversion<double>::toMpreal(const type & v, int /*precision*/) {
	return mpfr::mpreal(v);
}

//END double specializations
//BEGIN mpreal specializations

Conversion<mpfr::mpreal>::type
Conversion<mpfr::mpreal>::moveFrom(const mpq_class & v) {
	return mpfr::mpreal(v.get_mpq_t());
}

mpq_class
Conversion<mpfr::mpreal>::toMpq(const type & v) {
	mpf_t tmpf;
	::mpf_init(tmpf);
	::mpfr_get_f(tmpf, v.mpfr_srcptr(), ::mpfr_get_default_rounding_mode());
	
	mpq_t tmpq;
	::mpq_init(tmpq);
	::mpq_set_f(tmpq, tmpf);
	
	return mpq_class(tmpq);
}

const mpfr::mpreal &
Conversion<mpfr::mpreal>::toMpreal(const type & v, int /*precision*/) {
	return v;
}

//END mpreal specializations
//BEGIN mpq_class specializations

Conversion<mpq_class>::type &&
Conversion<mpq_class>::moveFrom(mpq_class && v) {
	return std::move(v);
}

const mpq_class &
Conversion<mpq_class>::toMpq(const Conversion<mpq_class>::type & v) {
	return v;
}

mpfr::mpreal Conversion<mpq_class>::toMpreal(const type & v, int precision) {
	return mpfr::mpreal(v.get_mpq_t(), precision);
}

//end mpq_class specializations

#if defined(LIB_RATSS_WITH_CGAL)

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

#endif

} //end namespace internal
}//end namespace LIB_RATSS_NAMESPACE