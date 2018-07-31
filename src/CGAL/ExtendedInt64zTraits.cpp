#include <libratss/CGAL/ExtendedInt64zTraits.h>

namespace CGAL {

namespace internal {
namespace imp {
namespace ExtendedInt64zTraits {

//BEGIN CGAL::Gmpz specializations
Make<CGAL::Gmpz>::type
Make<CGAL::Gmpz>::operator()(int64_t v) const {
	return type(LIB_RATSS_NAMESPACE::GmpTraits::signed_number(v));
}

Make<CGAL::Gmpz>::type
Make<CGAL::Gmpz>::operator()(uint64_t v) const {
	return type(LIB_RATSS_NAMESPACE::GmpTraits::unsigned_number(v));
}

bool
Fits<CGAL::Gmpz, uint64_t>::operator()(const type & value) const {
#ifdef __LP64__
	return ::mpz_fits_ulong_p(value.mpz());
#else
	return (value >> 64) == 0;
#endif
}

bool
Fits<CGAL::Gmpz, int64_t>::operator()(const type & value) const {
#ifdef __LP64__
	return ::mpz_fits_slong_p(value.mpz());
#else
	return Make<CGAL::Gmpz>()(ToIntegral<CGAL::Gmpz, int64_t>()(value)) == value;
#endif
}

ToIntegral<CGAL::Gmpz, uint64_t>::integral_type
ToIntegral<CGAL::Gmpz, uint64_t>::operator()(const type & value) const {
#ifdef __LP64__
	return ::mpz_get_ui(value.mpz());
#else
	return uint64_t(::mpz_get_ui(value.mpz())) | (uint64_t(::mpz_get_ui((value >> 32).mpz())) << 32);
#endif
}

ToIntegral<CGAL::Gmpz, int64_t>::integral_type
ToIntegral<CGAL::Gmpz, int64_t>::operator()(const type & value) const {
#ifdef __LP64__
	return ::mpz_get_si(value.mpz());
#else
	if (value < 0) {
		return - integral_type(ToIntegral<CGAL::Gmpz, uint64_t>()(value));
	}
	else {
		return integral_type(ToIntegral<CGAL::Gmpz, uint64_t>()(value));
	}
#endif
}
//END
//BEGIN boost_int1024 specializations

Make<boost_int1024>::type
Make<boost_int1024>::operator()(int64_t v) const {
	return type(LIB_RATSS_NAMESPACE::GmpTraits::signed_number(v));
}

Make<boost_int1024>::type
Make<boost_int1024>::operator()(uint64_t v) const {
	return type(LIB_RATSS_NAMESPACE::GmpTraits::unsigned_number(v));
}

//END

}}}//end namespace internal::imp::ExtendedInt64zTraits

	
} //end namespace
