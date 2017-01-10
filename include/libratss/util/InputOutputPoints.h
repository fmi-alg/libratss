#ifndef LIB_RATSS_UTIL_INPUT_OUTPUT_POINTS_H
#define LIB_RATSS_UTIL_INPUT_OUTPUT_POINTS_H
#pragma once

#include <libratss/constants.h>
#include <libratss/GeoCalc.h>

namespace LIB_RATSS_NAMESPACE {

struct InputOutputPointBase {
	typedef enum {
		FM_INVALID=0,
		FM_GEO=0x1, FM_SPHERICAL=0x2,
		FM_CARTESIAN_FLOAT=0x4, FM_CARTESIAN_FLOAT128=0x8,
		FM_CARTESIAN_RATIONAL=0x10, FM_CARTESIAN_SPLIT_RATIONAL=0x20,
		
		FM_FLOAT=FM_CARTESIAN_FLOAT, FM_FLOAT128=FM_CARTESIAN_FLOAT128,
		FM_RATIONAL=FM_CARTESIAN_RATIONAL, FM_SPLIT_RATIONAL=FM_CARTESIAN_SPLIT_RATIONAL
	} Format;
};

struct InputPoint: InputOutputPointBase {
	GeoCalc c;
	std::vector<mpfr::mpreal> coords;
	void normalize();
	void setPrecision(int precision);
	void assign(std::istream & is, Format fmt, int precision);
	template<typename T_ITERATOR>
	void assign(const T_ITERATOR & begin, const T_ITERATOR & end, int precision) {
		coords.clear();
		using value_type = typename std::iterator_traits<T_ITERATOR>::value_type;
		std::transform(begin, end, std::back_inserter(this->coords), [precision](const value_type &v) -> mpfr::mpreal {
			return Conversion<value_type>::toMpreal(v, precision);
		});
	}
	void print(std::ostream & out) const;
	mpfr::mpreal epsUpperBound() const;
	mpfr::mpreal sqLen() const;
};

struct OutputPoint: InputOutputPointBase {
	GeoCalc c;
	std::vector<mpq_class> coords;
	OutputPoint();
	template<typename T_ITERATOR>
	OutputPoint(const T_ITERATOR & begin, const T_ITERATOR & end) : coords(begin, end) {}
	template<typename T_VALUE>
	OutputPoint(const std::initializer_list<T_VALUE> & il) : coords(il.begin(), il.end()) {}
	OutputPoint(int dimension);
	OutputPoint(const OutputPoint & other);
	OutputPoint(OutputPoint && other);
	OutputPoint & operator=(const OutputPoint & other);
	OutputPoint & operator=(OutputPoint && other);
	void clear();
	void resize(std::size_t _n);
	void assign(std::istream & is, Format fmt, int precision);
	void print(std::ostream & out, Format fmt) const;
	bool valid() const;
};


}//end namespace LIB_RATSS_NAMESPACE


#endif