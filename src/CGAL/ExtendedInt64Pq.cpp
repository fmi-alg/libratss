#include <libratss/CGAL/ExtendedInt64Pq.h>
#include <stdexcept>

namespace CGAL {

template<>
uint64_t ExtendedInt64Pq<CGAL::Gmpq>::number_of_extended_allocations = 0;

template<>
uint64_t ExtendedInt64Pq<CGAL::Gmpq>::number_of_allocations = 0;

}//end namespace CGAL