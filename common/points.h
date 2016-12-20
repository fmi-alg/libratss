#ifndef LIB_RATSS_COMMON_POINTS_H
#define LIB_RATSS_COMMON_POINTS_H
#pragma once

#include <libratss/constants.h>
#include <libratss/Calc.h>

namespace LIB_RATSS_NAMESPACE {

struct InputPoint {
	Calc c;
	std::vector<mpfr::mpreal> coords;
	void normalize();
	void setPrecision(int precision);
	void assign(std::istream & is);
	void print(std::ostream & out) const;
	mpfr::mpreal epsUpperBound() const;
	mpfr::mpreal sqLen() const;
};

struct OutputPoint {
	typedef enum {FM_RATIONAL, FM_SPLIT_RATIONAL, FM_FLOAT, FM_FLOAT128} Format;
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
	void print(std::ostream & out, Format fmt) const;
	bool valid() const;
};


}//end namespace LIB_RATSS_NAMESPACE


#endif