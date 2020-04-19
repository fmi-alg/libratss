#ifndef LIB_RATSS_GEO_CALC_H
#define LIB_RATSS_GEO_CALC_H
#pragma once

#include <libratss/constants.h>
#include <libratss/Calc.h>

namespace LIB_RATSS_NAMESPACE {

class GeoCalc: public Calc {
public:
	bool isOnSphere(const mpfr::mpreal & mpdx, const mpfr::mpreal & mpdy, const mpfr::mpreal & mpdz) const;

	void spherical(const mpfr::mpreal & lat, const mpfr::mpreal & lon, mpfr::mpreal & theta, mpfr::mpreal & phi) const;
	void geo(const mpfr::mpreal & theta, const mpfr::mpreal & phi, mpfr::mpreal & lat, mpfr::mpreal & lon) const;
	
	///lat and lon are in DEGREE! -90 <= lat <= 90 && (0 <= lon <= 360 || -180 <= lon <= 180)
	void cartesian(const mpfr::mpreal & lat, const mpfr::mpreal & lon, mpfr::mpreal & x, mpfr::mpreal & y, mpfr::mpreal & z) const;
	void cartesianFromSpherical(const mpfr::mpreal & theta, const mpfr::mpreal & phi, mpfr::mpreal & x, mpfr::mpreal & y, mpfr::mpreal & z) const;

#if defined(LIB_RATSS_WITH_CORE_TWO)
	///lat and lon are in DEGREE! -90 <= lat <= 90 && (0 <= lon <= 360 || -180 <= lon <= 180)
	void cartesian(CORE_TWO::Expr const & lat, CORE_TWO::Expr const & lon, CORE_TWO::Expr & x, CORE_TWO::Expr & y, CORE_TWO::Expr & z) const;
	void cartesianFromSpherical(CORE_TWO::Expr const & theta, CORE_TWO::Expr const & phi, CORE_TWO::Expr & x, CORE_TWO::Expr & y, CORE_TWO::Expr & z) const;
#endif
	
	///theta, phi are in radiant
	void spherical(const mpfr::mpreal & x, const mpfr::mpreal & y, const mpfr::mpreal & z, mpfr::mpreal & theta, mpfr::mpreal & phi) const;
	
	///lat and lon are in DEGREE! -90 <= lat <= 90 && (0 <= lon <= 360 || -180 <= lon <= 180)
	void geo(const mpfr::mpreal & x, const mpfr::mpreal & y, const mpfr::mpreal & z, mpfr::mpreal & lat, mpfr::mpreal & lon) const;
};

}//end namespace LIB_RATSS_NAMESPACE


#endif
