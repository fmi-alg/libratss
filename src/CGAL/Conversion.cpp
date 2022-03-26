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

//BEGIN boost::multiprecision::number<boost::multiprecision::backends::gmp_rational> specilizations

Conversion<boost::multiprecision::number<boost::multiprecision::backends::gmp_rational>>::type
Conversion<boost::multiprecision::number<boost::multiprecision::backends::gmp_rational>>::moveFrom(const mpq_class & v) {
	return type( v.get_mpq_t() );
}

mpq_class
Conversion<boost::multiprecision::number<boost::multiprecision::backends::gmp_rational>>::toMpq(const type & v) {
	return mpq_class( v.backend().data() );
}

mpfr::mpreal
Conversion<boost::multiprecision::number<boost::multiprecision::backends::gmp_rational>>::toMpreal(const type & v, int precision) {
	return Conversion<mpq_class>::toMpreal( toMpq(v), precision );
}

//END boost::multiprecision::number<boost::multiprecision::backends::gmp_rational> specilizations

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
		mpq_class result(
			gmp_int64_t(v.numerator().get()), 
			gmp_uint64_t(v.denominator().get())
		);
		result.canonicalize();
		return result;
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
		mpq_class result(
			gmp_int64_t(v.numerator().get()),
			gmp_uint64_t(v.denominator().get())
		);
		result.canonicalize();
		return result;
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
//BEGIN CGAL::Gmpz specilizations

Conversion<CGAL::Gmpz>::type
Conversion<CGAL::Gmpz>::moveFrom(const mpq_class & v) {
	mpz_class tmp = v.get_num()/v.get_den();
	return CGAL::Gmpz(tmp.get_mpz_t());
}

mpq_class
Conversion<CGAL::Gmpz>::toMpq(const type & v) {
	return mpq_class(mpz_class(v.mpz()));
}

mpfr::mpreal
Conversion<CGAL::Gmpz>::toMpreal(const type & v, int precision) {
	return mpfr::mpreal(v.mpz(), precision);
}

//END CGAL::Gmpz specilizations

//BEGIN CORE::BigInt specilizations
mpq_class
Conversion<CORE::BigInt>::toMpq(const type & v) {
	return Conversion<mpz_class>::toMpq(mpz_class(v.get_mp()));
}

mpfr::mpreal
Conversion<CORE::BigInt>::toMpreal(const type & v, int precision) {
	return Conversion<mpz_class>::toMpreal(mpz_class(v.get_mp()), precision);
}
//END CORE::BigInt specilizations

//BEGIN CORE::BigFloat specilizations
mpq_class
Conversion<CORE::BigFloat>::toMpq(const type & v) {
	return mpq_class( Conversion<CORE::BigRat>::toMpq(v.BigRatValue()) );
}

mpfr::mpreal
Conversion<CORE::BigFloat>::toMpreal(const type & v, int precision) {
	if (precision < 0) {
		precision = mpz_sizeinbase(v.m().get_mp(), 2);
	}
	mpfr::mpreal tmp(v.m().get_mp(), precision);
	//BigFloat has a different exponent style. See Core Library Tutorial page 25 or
	//take a look at the ctor of BigFloatRep(BigInt, long) in BigFloatRep.h:281 (in CGAL 4.14)
	tmp.set_exp(tmp.get_exp() + v.getRep().bits(v.exp()));
	return tmp;
}
//END CORE::BigInt specilizations

//BEGIN CORE::BigRat specilizations
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
//END CORE::BigRat specilizations

//BEGIN Exact_spherical_kernel_3::Root_of_2 specilizations
Conversion<CGAL::Exact_spherical_kernel_3::Root_of_2>::type
Conversion<CGAL::Exact_spherical_kernel_3::Root_of_2>::moveFrom(const mpq_class & v) {
	return CGAL::Exact_spherical_kernel_3::Root_of_2( Conversion<CGAL::Exact_spherical_kernel_3::FT>::moveFrom(v) );
}

mpq_class
Conversion<CGAL::Exact_spherical_kernel_3::Root_of_2>::toMpq(const type & v, int precision) {
	if (precision < 53) {
		return mpq_class( CGAL::to_double(v) );
	}
	else {
		throw std::runtime_error("Conversion<CGAL::Exact_spherical_kernel_3::Root_of_2>::toMpq: precision is too high: " + std::to_string(precision) + "!< 53");
		return mpq_class();
	}
}

mpfr::mpreal
Conversion<CGAL::Exact_spherical_kernel_3::Root_of_2>::toMpreal(const type & v, int precision) {
	if (precision < 53) {
		return mpfr::mpreal( CGAL::to_double(v) );
	}
	else {
		throw std::runtime_error("CGAL::Conversion<Exact_spherical_kernel_3::Root_of_2>::toMpreal: precision is too high " + std::to_string(precision) + "!< 53");
		return mpfr::mpreal();
	}
}
//END Exact_spherical_kernel_3::Root_of_2 specilizations

//BEGIN CORE::Expr specilizations
Conversion<CORE::Expr>::type
Conversion<CORE::Expr>::moveFrom(const mpq_class & v) {
	return CORE::Expr( CORE::BigRat(v.get_mpq_t()) );
}

mpq_class
Conversion<CORE::Expr>::toMpq(const type & v, int precision) {
	if (precision < 0) {
		return mpq_class(v.BigRatValue().get_mp());
	}
	else {
		return toMpq(v.approx(precision, precision), -1);
	}
}

mpfr::mpreal
Conversion<CORE::Expr>::toMpreal(const type & v, int precision) {
	if (precision < 0) {
		return Conversion<CORE::BigFloat>::toMpreal(v.BigFloatValue(), precision);
	}
	else {
		return Conversion<CORE::BigFloat>::toMpreal(v.approx(precision, precision).BigFloatValue(), precision);
	}
}
//END CORE::Expr specilizations

//BEGIN CORE::Real specilizations
Conversion<CORE::Real>::type
Conversion<CORE::Real>::moveFrom(const mpq_class &v) {
	return type( Conversion<CORE::BigRat>::moveFrom(v) );
}

mpq_class
Conversion<CORE::Real>::toMpq(const type & v, int precision) {
	if (precision < 0) {
		return mpq_class( Conversion<CORE::BigRat>::toMpq(v.BigRatValue()) );
	}
	else {
		return toMpq(v.approx(precision, precision), -1);
	}
}

mpfr::mpreal
Conversion<CORE::Real>::toMpreal(const type & v, int precision) {
	if (precision < 0) {
		return Conversion<CORE::BigFloat>::toMpreal(v.BigFloatValue(), precision);
	}
	else {
		return Conversion<CORE::BigFloat>::toMpreal(v.approx(precision, precision).BigFloatValue(), precision);
	}
}
//END CORE::Real specilizations

//BEGIN CGAL::Lazy_exact_nt<mpq_class>

#if CGAL_VERSION_NR >= 1041101000 && defined(CGAL_USE_GMPXX)

Conversion< CGAL::Lazy_exact_nt<mpq_class> >::type
Conversion< CGAL::Lazy_exact_nt<mpq_class> >::moveFrom(mpq_class const & v) {
	return type( v );
}

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
