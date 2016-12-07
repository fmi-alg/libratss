#include <libratss/Conversion.h>
#include <libratss/mpreal.h>

namespace LIB_RATSS_NAMESPACE {

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

//END mpq_class specializations


}//end namespace LIB_RATSS_NAMESPACE