#include <libratss/CGAL/ExtendedInt64q.h>
namespace CGAL {

template<>
uint64_t ExtendedInt64q<CGAL::Gmpq>::number_of_extended_allocations = 0;

template<>
uint64_t ExtendedInt64q<CGAL::Gmpq>::number_of_allocations = 0;

template<>
uint32_t ExtendedInt64q<CGAL::Gmpq>::max_numerator_bits = 0;

template<>
uint32_t ExtendedInt64q<CGAL::Gmpq>::max_denominator_bits = 0;

template<>
uint64_t ExtendedInt64q<CGAL::internal::boost_int1024q>::number_of_extended_allocations = 0;

template<>
uint64_t ExtendedInt64q<CGAL::internal::boost_int1024q>::number_of_allocations = 0;

template<>
uint32_t ExtendedInt64q<CGAL::internal::boost_int1024q>::max_numerator_bits = 0;

template<>
uint32_t ExtendedInt64q<CGAL::internal::boost_int1024q>::max_denominator_bits = 0;

namespace internal {

void
ExtendedInt64qTraits<CGAL::Gmpq>::simplify(type & v)
{
	::mpq_canonicalize(v.mpq());
}

bool
ExtendedInt64qTraits<CGAL::Gmpq>::fits_int64(const numerator_type & v)
{
	return ::mpz_fits_slong_p(v.mpz());
}


int64_t
ExtendedInt64qTraits<CGAL::Gmpq>::to_int64(const numerator_type & v) {
	return ::mpz_get_si(v.mpz());
}

double
ExtendedInt64qTraits<CGAL::Gmpq>::to_double(const type & v)
{
	return v.to_double();
}

ExtendedInt64qTraits<CGAL::Gmpq>::numerator_type
ExtendedInt64qTraits<CGAL::Gmpq>::numerator(const type & v)
{
	return v.numerator();
}

ExtendedInt64qTraits<CGAL::Gmpq>::denominator_type
ExtendedInt64qTraits<CGAL::Gmpq>::denominator(const type & v) {
	return v.denominator();
}

CGAL::ExtendedInt64z::extension_type const &
ExtendedInt64qTraits<CGAL::Gmpq>::to_ei64z(const numerator_type & v) {
	return v;
}


uint32_t
ExtendedInt64qTraits<CGAL::Gmpq>::num_bits(const numerator_type & v) {
	return uint32_t(::mpz_sizeinbase(v.mpz(), 2));
}

ExtendedInt64qTraits<CGAL::Gmpq>::type
ExtendedInt64qTraits<CGAL::Gmpq>::make(base_type numerator, base_type denominator) {
	return type(LIB_RATSS_NAMESPACE::GmpTraits::signed_number(numerator), LIB_RATSS_NAMESPACE::GmpTraits::signed_number(denominator));
}

ExtendedInt64qTraits<CGAL::Gmpq>::type
ExtendedInt64qTraits<CGAL::Gmpq>::make(base_type numerator, unsigned_base_type denominator) {
	return type(LIB_RATSS_NAMESPACE::GmpTraits::signed_number(numerator), LIB_RATSS_NAMESPACE::GmpTraits::unsigned_number(denominator));
}

//BEGIN boost_int1024q
void
ExtendedInt64qTraits<boost_int1024q>::simplify(type & /*v*/)
{}

bool
ExtendedInt64qTraits<boost_int1024q>::fits_int64(const numerator_type & v)
{
	return std::numeric_limits<int64_t>::min() <= v && v <= std::numeric_limits<int64_t>::max();
}

int64_t
ExtendedInt64qTraits<boost_int1024q>::to_int64(const numerator_type & v) {
	return v.convert_to<int64_t>();
}

double
ExtendedInt64qTraits<boost_int1024q>::to_double(const type & v)
{
	return v.convert_to<double>();
}

ExtendedInt64qTraits<boost_int1024q>::numerator_type
ExtendedInt64qTraits<boost_int1024q>::numerator(const type & v)
{
	return boost::multiprecision::numerator(v);
}

ExtendedInt64qTraits<boost_int1024q>::denominator_type
ExtendedInt64qTraits<boost_int1024q>::denominator(const type & v) {
	return boost::multiprecision::denominator(v);
}

CGAL::ExtendedInt64z::extension_type
ExtendedInt64qTraits<boost_int1024q>::to_ei64z(const numerator_type & v) {
	return CGAL::ExtendedInt64z::extension_type( v.str() );
}

uint32_t
ExtendedInt64qTraits<boost_int1024q>::num_bits(const numerator_type &) {
	return 1024;
}

ExtendedInt64qTraits<boost_int1024q>::type
ExtendedInt64qTraits<boost_int1024q>::make(base_type numerator, base_type denominator) {
	return type(numerator, denominator);
}

ExtendedInt64qTraits<boost_int1024q>::type
ExtendedInt64qTraits<boost_int1024q>::make(base_type numerator, unsigned_base_type denominator) {
	if (numerator < 0) {
		return -type(unsigned_base_type(-numerator), denominator);
	}
	else {
		return type(unsigned_base_type(numerator), denominator);
	}
}

}//end namespace internal
}//end namespace CGAL
