#include <libratss/Conversion.h>
#include <libratss/core2/Conversion.h>
#include <libratss/types.h>

namespace LIB_RATSS_NAMESPACE {

//BEGIN CORE_TWO::BigInt specilizations
mpq_class
Conversion<CORE_TWO::BigInt>::toMpq(const type & v) {
	return Conversion<mpz_class>::toMpq(mpz_class(v.mp()));
}

mpfr::mpreal
Conversion<CORE_TWO::BigInt>::toMpreal(const type & v, int precision) {
	return Conversion<mpz_class>::toMpreal(mpz_class(v.mp()), precision);
}
//END CORE_TWO::BigInt specilizations

//BEGIN CORE_TWO::BigRat specilizations
Conversion<CORE_TWO::BigRat>::type
Conversion<CORE_TWO::BigRat>::moveFrom(const mpq_class &v) {
	return CORE_TWO::BigRat(v.get_mpq_t());
}

mpq_class
Conversion<CORE_TWO::BigRat>::toMpq(const type & v) {
	return mpq_class(v.mp());
}

mpfr::mpreal
Conversion<CORE_TWO::BigRat>::toMpreal(const type & v, int precision) {
	return mpfr::mpreal(v.mp(), precision);
}
//END CORE_TWO::BigRat specilizations

//BEGIN CORE_TWO::BigFloat specilizations
mpq_class
Conversion<CORE_TWO::BigFloat>::toMpq(const type & v) {
	return Conversion<mpfr::mpreal>::toMpq(mpfr::mpreal(v.mp()));
}

mpfr::mpreal
Conversion<CORE_TWO::BigFloat>::toMpreal(const type & v, int precision) {
	return Conversion<mpfr::mpreal>::toMpreal(mpfr::mpreal(v.mp()), precision);
}
//END CORE_TWO::BigFloat specilizations

//BEGIN CORE_TWO::BigFloat2 specilizations
mpq_class
Conversion<CORE_TWO::BigFloat2>::toMpq(const type & v) {
	return Conversion<CORE_TWO::BigFloat>::toMpq(v.get_f());
}

mpfr::mpreal
Conversion<CORE_TWO::BigFloat2>::toMpreal(const type & v, int precision) {
	return Conversion<CORE_TWO::BigFloat>::toMpreal(v.get_f(), precision);
}
//END CORE_TWO::BigFloat specilizations

//BEGIN CORE_TWO::Expr specilizations
Conversion<CORE_TWO::Expr>::type
Conversion<CORE_TWO::Expr>::moveFrom(const mpq_class & v) {
	return CORE_TWO::Expr( CORE_TWO::BigRat(v.get_mpq_t()) );
}

mpq_class
Conversion<CORE_TWO::Expr>::toMpq(const type & v, int precision) {
	if (precision < 0) {
		return mpq_class(v.BigRatValue().mp());
	}
	else {
		type cp = v;
		return toMpq(cp.approx(precision, precision), -1);
	}
}

mpfr::mpreal
Conversion<CORE_TWO::Expr>::toMpreal(const type & v, int precision) {
	if (precision < 0) {
		return Conversion<CORE_TWO::BigFloat>::toMpreal(v.BigFloatValue(), precision);
	}
	else {
		type cp = v;
		return Conversion<CORE_TWO::BigFloat>::toMpreal(cp.approx(precision, precision), precision);
	}
}
//END CORE_TWO::Expr specilizations

}
