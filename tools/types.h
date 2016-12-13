#ifndef LIB_RATSS_TOOLS_TYPES_H
#define LIB_RATSS_TOOLS_TYPES_H
#pragma once

#include <vector>
#include <gmpxx.h>

#include <libratss/constants.h>
#include <libratss/Conversion.h>
#include <libratss/Calc.h>

namespace LIB_RATSS_NAMESPACE {
namespace tools {


struct InputPoint {
	Calc c;
	std::vector<mpfr::mpreal> coords;
	void normalize();
	void setPrecision(int precision);
	void assign(std::istream & is);
	void print(std::ostream & out) const;
	mpfr::mpreal sqLen();
};

struct OutputPoint {
	typedef enum {FM_RATIONAL, FM_SPLIT_RATIONAL, FM_FLOAT} Format;
	std::vector<mpq_class> coords;
	OutputPoint();
	OutputPoint(int dimension);
	OutputPoint(const OutputPoint & other);
	OutputPoint(OutputPoint && other);
	OutputPoint & operator=(const OutputPoint & other);
	OutputPoint & operator=(OutputPoint && other);
	void clear();
	void resize(std::size_t _n);
	void print(std::ostream & out, Format fmt) const;
};

}}//end LIB_RATSS_NAMESPACE


#endif