#ifndef LIB_RATSS_COMMON_STATS_H
#define LIB_RATSS_COMMON_STATS_H
#pragma once

#include <libratss/constants.h>
#include <limits>
#include <ostream>
#include <gmpxx.h>

namespace LIB_RATSS_NAMESPACE {

struct BitCount {
	std::size_t count;
	std::size_t maxNofBitsNum;
	std::size_t maxNofBitsDenom;
	std::size_t sumNofBitsNum;
	std::size_t sumNofBitsDenom;
	BitCount();
	void update(mpq_class v);
	void print(std::ostream & out) const;
};

std::ostream & operator<<(std::ostream& out, const BitCount & bc);

}//end namespace LIB_RATSS_NAMESPACE


#endif