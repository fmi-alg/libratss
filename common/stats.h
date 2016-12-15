#ifndef LIB_RATSS_COMMON_STATS_H
#define LIB_RATSS_COMMON_STATS_H
#pragma once

#include <libratss/constants.h>
#include <limits>
#include <ostream>
#include <gmpxx.h>

namespace LIB_RATSS_NAMESPACE {

template<typename FT>
class MinMaxMeanStats {
public:
	MinMaxMeanStats() : m_count(0), m_min(std::numeric_limits<FT>::max()), m_max(std::numeric_limits<FT>::min()), m_sum(0) {}
	void update(const FT & ft) {
		using std::min;
		using std::max;
		++m_count;
		m_min = min(m_min, ft);
		m_max = max(m_max, ft);
		m_sum += ft;
	}
	FT min() const { return m_min; }
	FT max() const { return m_max; }
	FT mean() const { return (m_count ? m_sum/m_count : FT(0)); }
	FT sum() const { return m_sum; }
	void print(std::ostream & out, const std::string & prefix) const {
		out << prefix << "min: " << min() << '\n';
		out << prefix << "max: " << max() << '\n';
		out << prefix << "mean: " << mean();
	}
private:
	std::size_t m_count;
	FT m_min;
	FT m_max;
	FT m_sum;
};

struct BitCount {
	using MyMinMaxMean = MinMaxMeanStats<std::size_t>;
	MyMinMaxMean numBits;
	MyMinMaxMean denomBits;
	MyMinMaxMean numLimbs;
	MyMinMaxMean denomLimbs;
	
	BitCount();
	void update(mpq_class v);
	template<typename T_INPUT_ITERATOR>
	void update(T_INPUT_ITERATOR begin, const T_INPUT_ITERATOR & end);
	void print(std::ostream & out) const;
};

std::ostream & operator<<(std::ostream& out, const BitCount & bc);

}//end namespace LIB_RATSS_NAMESPACE

namespace LIB_RATSS_NAMESPACE {


template<typename T_INPUT_ITERATOR>
void BitCount::update(T_INPUT_ITERATOR begin, const T_INPUT_ITERATOR & end) {
	for(; begin != end; ++begin) {
		update(*begin);
	}
}

}//end namespace LIB_RATSS_NAMESPACE


#endif