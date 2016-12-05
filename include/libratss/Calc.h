#ifndef LIB_RATSS_CALC_H
#define LIB_RATSS_CALC_H
#pragma once

#include <libratss/constants.h>
#include <libratss/Conversion.h>

namespace LIB_RATSS_NAMESPACE {

struct Calc {

	mpfr::mpreal sin(const mpfr::mpreal & v) const;
	mpfr::mpreal asin(const mpfr::mpreal & v) const;

	mpfr::mpreal cos(const mpfr::mpreal & v) const;
	mpfr::mpreal acos(const mpfr::mpreal & v) const;
	
	mpfr::mpreal tan(const mpfr::mpreal & v) const;
	mpfr::mpreal atan(const mpfr::mpreal & v) const;
	
	mpfr::mpreal squaredDistance(const mpfr::mpreal & a, const mpfr::mpreal & b) const;
	mpfr::mpreal squaredDistance(const mpfr::mpreal & a, const mpfr::mpreal & b, const mpfr::mpreal & c) const;
	mpfr::mpreal mult(const mpfr::mpreal & a, const mpfr::mpreal & b) const;
	mpfr::mpreal div(const mpfr::mpreal & a, const mpfr::mpreal & b) const;
	mpfr::mpreal sq(const mpfr::mpreal & v) const;
	mpfr::mpreal sqrt(const mpfr::mpreal & v) const;
	mpfr::mpreal add(const mpfr::mpreal & a, const mpfr::mpreal & b)  const;
	mpfr::mpreal sub(const mpfr::mpreal & a, const mpfr::mpreal & b)  const;
	bool isOnSphere(const mpfr::mpreal & mpdx, const mpfr::mpreal & mpdy, const mpfr::mpreal & mpdz) const;

	///@return r a number satisfying the following conditions:
	/// r is a fraction with the smallest denominator such that lower <= r <= upper
	mpq_class within(const mpq_class& lower, const mpq_class& upper);
	
	///snap to a fraction that is within the precision of the given float
	mpq_class snap(const mpfr::mpreal & v);
	
	void spherical(const mpfr::mpreal & lat, const mpfr::mpreal & lon, mpfr::mpreal & theta, mpfr::mpreal & phi) const;
	void geo(const mpfr::mpreal & theta, const mpfr::mpreal & phi, mpfr::mpreal & lat, mpfr::mpreal & lon) const;
	
	///lat and lon are in DEGREE! -90 <= lat <= 90 && (0 <= lon <= 360 || -180 <= lon <= 180)
	void cartesian(const mpfr::mpreal & lat, const mpfr::mpreal & lon, mpfr::mpreal & x, mpfr::mpreal & y, mpfr::mpreal & z) const;
	void cartesianFromSpherical(const mpfr::mpreal & theta, const mpfr::mpreal & phi, mpfr::mpreal & x, mpfr::mpreal & y, mpfr::mpreal & z) const;
	
	///theta, phi are in radiant
	void spherical(const mpfr::mpreal & x, const mpfr::mpreal & y, const mpfr::mpreal & z, mpfr::mpreal & theta, mpfr::mpreal & phi) const;
	
	///lat and lon are in DEGREE! -90 <= lat <= 90 && (0 <= lon <= 360 || -180 <= lon <= 180)
	void geo(const mpfr::mpreal & x, const mpfr::mpreal & y, const mpfr::mpreal & z, mpfr::mpreal & lat, mpfr::mpreal & lon) const;
	
	std::size_t maxBitCount(const mpq_class &v) const;
};

}//end namespace LIB_RATSS_NAMESPACE

#endif