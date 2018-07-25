#ifndef LIB_RATSS_CGAL_TYPES_H
#define LIB_RATSS_CGAL_TYPES_H
#pragma once

#include <libratss/constants.h>

namespace LIB_RATSS_NAMESPACE {
	
struct GmpTraits {
	using unsigned_number = unsigned long int;
	using signed_number = signed long int;
	static constexpr bool has64 = (sizeof(uint64_t) <= sizeof(unsigned_number)) && (sizeof(int64_t) <= sizeof(unsigned_number));
};

// using gmp_uint64_t = unsigned long int;
// using gmp_int64_t = signed long int;
// static_assert(sizeof(uint64_t) <= sizeof(gmp_uint64_t), "GMP has no constructor for 64 Bit unsigned numbers");
// static_assert(sizeof(int64_t) <= sizeof(gmp_int64_t), "GMP has no constructor for 64 Bit signed numbers");

}//end namespace


#endif
