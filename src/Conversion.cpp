#include <libratss/Conversion.h>
#include <libratss/mpreal.h>
#include <libratss/types.h>

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
//BEGIN uint32_t specializations

Conversion<uint32_t>::type
Conversion<uint32_t>::moveFrom(const mpq_class & v) {
	return ((mpz_class)v).get_ui();
}

mpq_class
Conversion<uint32_t>::toMpq(const type & v) {
	return mpq_class(gmp_uint64_t(v));
}

mpfr::mpreal
Conversion<uint32_t>::toMpreal(const type & v, int /*precision*/) {
	return mpfr::mpreal(v);
}

//END uint32_t specializations
//BEGIN int32_t specializations

Conversion<int32_t>::type
Conversion<int32_t>::moveFrom(const mpq_class & v) {
	return ((mpz_class)v).get_ui();
}

mpq_class
Conversion<int32_t>::toMpq(const type & v) {
	return mpq_class(gmp_int64_t(v));
}

mpfr::mpreal
Conversion<int32_t>::toMpreal(const type & v, int /*precision*/) {
	return mpfr::mpreal(v);
}

//END uint32_t specializations
//BEGIN uint64_t specializations

Conversion<uint64_t>::type
Conversion<uint64_t>::moveFrom(const mpq_class & v) {
	return ((mpz_class)v).get_ui();
}

mpq_class
Conversion<uint64_t>::toMpq(const type & v) {
	return mpq_class(gmp_uint64_t(v));
}

mpfr::mpreal
Conversion<uint64_t>::toMpreal(const type & v, int /*precision*/) {
	return mpfr::mpreal(v);
}

//END uint64_t specializations
//BEGIN int64_t specializations

Conversion<int64_t>::type
Conversion<int64_t>::moveFrom(const mpq_class & v) {
	return ((mpz_class)v).get_si();
}

mpq_class
Conversion<int64_t>::toMpq(const type & v) {
	return mpq_class(gmp_int64_t(v));
}

mpfr::mpreal
Conversion<int64_t>::toMpreal(const type & v, int /*precision*/) {
	return mpfr::mpreal(v);
}

//END int64_t specializations
//BEGIN mpreal specializations

Conversion<mpfr::mpreal>::type
Conversion<mpfr::mpreal>::moveFrom(const mpq_class & v) {
	return mpfr::mpreal(v.get_mpq_t());
}

mpq_class
Conversion<mpfr::mpreal>::toMpq(const type & v) {
	if (!isfinite(v)) {
		throw std::overflow_error("Conversion<mpfr::mpreal>: Cannot convert infinite value to rational");
	}
	mpf_class tmpf;
	::mpfr_get_f(tmpf.get_mpf_t(), v.mpfr_srcptr(), MPFR_RNDZ);
	
	mpq_class result(tmpf);
	return result;
}

const mpfr::mpreal &
Conversion<mpfr::mpreal>::toMpreal(const type & v, int /*precision*/) {
	return v;
}

//END mpreal specializations
//BEGIN mpq_class specializations

Conversion<mpq_class>::type &&
Conversion<mpq_class>::moveFrom(mpq_class & v) {
	return std::move(v);
}

Conversion<mpq_class>::type &&
Conversion<mpq_class>::moveFrom(mpq_class && v) {
	return std::move(v);
}

const mpq_class &
Conversion<mpq_class>::toMpq(const type & v) {
	return v;
}

mpfr::mpreal Conversion<mpq_class>::toMpreal(const type & v, int precision) {
	return mpfr::mpreal(v.get_mpq_t(), precision);
}

//END mpq_class specializations
//BEGIN mpz_class specializations

mpq_class
Conversion<mpz_class>::toMpq(const type & v) {
	return mpq_class(v, mpz_class(1));
}

mpfr::mpreal Conversion<mpz_class>::toMpreal(const type & v, int precision) {
	return mpfr::mpreal(v.get_mpz_t(), precision);
}
//END mpz_class specializations


}//end namespace LIB_RATSS_NAMESPACE
