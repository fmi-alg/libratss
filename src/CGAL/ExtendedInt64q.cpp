#include <libratss/CGAL/ExtendedInt64q.h>
#include <stdexcept>

namespace CGAL {

template<>
uint64_t ExtendedInt64q<CGAL::Gmpq>::number_of_extended_allocations = 0;

template<>
uint64_t ExtendedInt64q<CGAL::Gmpq>::number_of_allocations = 0;

namespace internal {

void
ExtendedInt64qTraits<CGAL::Gmpq>::simplify(type & v)
{
	::mpq_canonicalize(v.mpq());
}

bool
ExtendedInt64qTraits<CGAL::Gmpq>::fits_int64(const CGAL::Gmpz & v)
{
	return ::mpz_fits_slong_p(v.mpz());
}


int64_t
ExtendedInt64qTraits<CGAL::Gmpq>::to_int64(const CGAL::Gmpz & v) {
	return ::mpz_get_si(v.mpz());
}

double
ExtendedInt64qTraits<CGAL::Gmpq>::to_double(const CGAL::Gmpz & v)
{
	return v.to_double();
}

}//end namespace internal
}//end namespace CGAL