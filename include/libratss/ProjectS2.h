#ifndef LIB_RATSS_PROJECT_S2_H
#define LIB_RATSS_PROJECT_S2_H
#pragma once

#include <libratss/constants.h>
#include <libratss/GeoCalc.h>
#include <libratss/ProjectSN.h>
#include <libratss/internal/ReferenceWrapper.h>
#include <assert.h>


namespace LIB_RATSS_NAMESPACE {

class ProjectS2: public ProjectSN {
public:
	using ProjectSN::sphere2Plane;
	using ProjectSN::plane2Sphere;

	template<typename T_FT>
	PositionOnSphere sphere2Plane(const T_FT& xs, const T_FT& ys, const T_FT& zs, T_FT& xp, T_FT& yp, T_FT& zp, PositionOnSphere pos = SP_INVALID) const WARN_UNUSED_RESULT;

	template<typename T_FT>
	void plane2Sphere(const T_FT & xp, const T_FT & yp, const T_FT & zp, PositionOnSphere pos, T_FT & xs, T_FT & ys, T_FT & zs) const;
public:
	using ProjectSN::snap;
	void snap(const mpfr::mpreal& flxs, const mpfr::mpreal& flys, const mpfr::mpreal& flzs, mpq_class& xs, mpq_class& ys, mpq_class& zs, int significands, int snapType = ST_FX | ST_PLANE | ST_NORMALIZE) const;
public:
	///lat and lon are in DEGREE!
	///This function projects coordinates that are given in spherical coordinates on to the sphere
	///using the stereographic projection as basis
	///all trigonometric functions are calculated using mpfr
	///You can give the precision of these calculation, all other precisions are then derived from it
	///lat and lon are in DEGREE! -90 <= lat <= 90 && (0 <= lon <= 360 || -180 <= lon <= 180)
	template<typename T_FT>
	void projectFromGeo(mpfr::mpreal lat, mpfr::mpreal lon, T_FT &xs, T_FT &ys, T_FT &zs, int precision = -1, int snapType = ST_FX | ST_PLANE | ST_NORMALIZE) const;

	///the same as projectFromGeo except that one can set the desired maximum distance
	template<typename T_FT>
	double projectFromGeo(mpfr::mpreal lat, mpfr::mpreal lon, T_FT &xs, T_FT &ys, T_FT &zs, double maxDist, int maxPrecision) const;
	
	template<typename T_FT>
	void projectFromSpherical(mpfr::mpreal theta, mpfr::mpreal phi, T_FT &xs, T_FT &ys, T_FT &zs, int precision = -1, int snapType = ST_FX | ST_PLANE | ST_NORMALIZE) const;

	///the same as projectFromGeo except that one can set the desired maximum distance
	template<typename T_FT>
	double projectFromSpherical(mpfr::mpreal theta, mpfr::mpreal phi, T_FT &xs, T_FT &ys, T_FT &zs, double maxDist, int maxPrecision) const;
	
	///project to lat/lon
	template<typename T_FT>
	void toGeo(const T_FT & xs, const T_FT & ys, const T_FT & zs, double & lat, double & lon, int precision) const;
	
	template<typename T_FT>
	void toSpherical(const T_FT & xs, const T_FT & ys, const T_FT & zs, double & theta, double & phi, int precision) const;
	
public:
	inline const GeoCalc & calc() const { return m_calc; }
private:
	GeoCalc m_calc;
};


}//end namespace LIB_RATSS_NAMESPACE


//below are the template definitions
namespace LIB_RATSS_NAMESPACE {

template<typename T_FT>
void ProjectS2::plane2Sphere(const T_FT & xp, const T_FT & yp, const T_FT & zp, PositionOnSphere pos, T_FT & xs, T_FT & ys, T_FT & zs) const {
	using ConstRefWrap = internal::ReferenceWrapper<const T_FT>;
	using RefWrap = internal::ReferenceWrapper<T_FT>;
	using ConstRefWrapIt = internal::ReferenceWrapperIterator<ConstRefWrap * >;
	using RefWrapIt = internal::ReferenceWrapperIterator<RefWrap *>;
	
	ConstRefWrap input[3] = {xp, yp, zp};
	RefWrap output[3] = {xs, ys, zs};
	ProjectSN::plane2Sphere(ConstRefWrapIt(input), ConstRefWrapIt(input+3), pos, RefWrapIt(output));
}

template<typename T_FT>
PositionOnSphere ProjectS2::sphere2Plane(const T_FT & xs, const T_FT & ys, const T_FT & zs, T_FT & xp, T_FT & yp, T_FT & zp, PositionOnSphere pos) const {
	using ConstRefWrap = internal::ReferenceWrapper<const T_FT>;
	using RefWrap = internal::ReferenceWrapper<T_FT>;
	using ConstRefWrapIt = internal::ReferenceWrapperIterator<ConstRefWrap * >;
	using RefWrapIt = internal::ReferenceWrapperIterator<RefWrap *>;
	
	ConstRefWrap input[3] = { xs, ys, zs};
	RefWrap output[3] = {xp, yp, zp};
	ConstRefWrapIt inputBegin(input);
	ConstRefWrapIt inputEnd(input+3);
	RefWrapIt outputBegin(output);
	return ProjectSN::sphere2Plane<ConstRefWrapIt, RefWrapIt>(inputBegin, inputEnd, outputBegin, pos);
}

template<typename T_FT>
void ProjectS2::projectFromGeo(mpfr::mpreal lat, mpfr::mpreal lon, T_FT& xs, T_FT& ys, T_FT& zs, int precision, int snapType) const {
	if (precision < 0) {
		precision = std::max<int>(lat.getPrecision(), lon.getPrecision());
	}

	mpfr::mpreal flxs, flys, flzs;
	mpq_class xpq, ypq, zpq;

	int tmpPrec = 4*std::max<int>( std::max<int>(lat.getPrecision(), lon.getPrecision()), precision );
	lat.setPrecision(tmpPrec);
	lon.setPrecision(tmpPrec);

	//clip to 3D and snap to sphere
	m_calc.cartesian(lat, lon, flxs, flys, flzs);
	snap(flxs, flys, flzs, xpq, ypq, zpq, precision, snapType);
	
	assert(!(lat > 0) || zs >= 0);
	xs = Conversion<T_FT>::moveFrom( std::move(xpq) );
	ys = Conversion<T_FT>::moveFrom( std::move(ypq) );
	zs = Conversion<T_FT>::moveFrom( std::move(zpq) );
}

template<typename T_FT>
double ProjectS2::projectFromGeo(mpfr::mpreal lat, mpfr::mpreal lon, T_FT &xs, T_FT &ys, T_FT &zs, double maxDist, int maxPrecision) const {
	int minSqPrec = std::max<int>(maxPrecision, 256);
	mpfr::mpreal xf, yf, zf, xfs, yfs, zfs, sqd(std::numeric_limits<double>::max());
	{
		if (minSqPrec < std::max<int>(lat.getPrecision(), lon.getPrecision())) {
			m_calc.cartesian(lat, lon, xf, yf, zf);
		}
		else {
			mpfr::mpreal lattmp(lat), lontmp(lon);
			lattmp.setPrecision(minSqPrec);
			lontmp.setPrecision(minSqPrec);
			m_calc.cartesian(lattmp, lontmp, xf, yf, zf);
		}
	}

	mpfr::mpreal mD2(m_calc.sq(mpfr::mpreal(maxDist)));
	
	//return -1 if prec is too low, 0 if it's okay and 1 if it is too high
	auto proj = [&, this](int prec) -> int {
		projectFromGeo(lat, lon, xs, ys, zs, prec);
		xfs = Conversion<T_FT>::toMpreal(xs, 2*prec);
		yfs = Conversion<T_FT>::toMpreal(ys, 2*prec);
		zfs = Conversion<T_FT>::toMpreal(zs, 2*prec);
		sqd = m_calc.squaredDistance(m_calc.sub(xfs, xf), m_calc.sub(yfs, yf), m_calc.sub(zfs, zf));
		if (sqd < mD2) {
			return 1;
		}
		else {
			return -1;
		}
	};
	
	int lowerPrec = 64;
	
	//first try up to 256 bits (which should suffice for 99% of all calls)
	for (int maxPrec(std::min<int>(256, maxPrecision)); lowerPrec <= maxPrec; lowerPrec += 64) {
		if (proj(64) == 1) {
			return m_calc.sqrt(sqd).toDouble();
		}
	}
	
	//nothing found so far, switch to binary search
	
	//we increment in strides of 64 since gmp uses machine-word for mp_limb_t
	//since this should be used on 64 bit machines we'll use 64 bits here
	
	int higherPrec = maxPrecision;
	int curPrec = (lowerPrec + higherPrec) / 2;
	curPrec = ((curPrec/64) + (curPrec % 64 == 0 ? 0 : 1))*64;
	int prevPrec = 0;
	
	while (true) {
		prevPrec = curPrec;
		int r = proj(curPrec);
		if (r < 0) { //not good enough
			lowerPrec = curPrec;
			curPrec = (lowerPrec + higherPrec) / 2;
			curPrec = ((curPrec/64) + (curPrec % 64 == 0 ? 0 : 1))*64;
		}
		else { //too many bits
			higherPrec = curPrec;
			curPrec = (lowerPrec + higherPrec) / 2;
			curPrec = ((curPrec/64) + (curPrec % 64 == 0 ? 0 : 1))*64;
		}
		
		if (prevPrec == curPrec) {
			break;
		}
	}
	
	return m_calc.sqrt(sqd).toDouble();
}

template<typename T_FT>
void ProjectS2::projectFromSpherical(mpfr::mpreal theta, mpfr::mpreal phi, T_FT& xs, T_FT& ys, T_FT& zs, int precision, int snapType) const {
	if (precision < 0) {
		precision = std::max<int>(theta.getPrecision(), phi.getPrecision());
	}

	mpq_class xpq, ypq, zpq;
	mpfr::mpreal flxs, flys, flzs;
	
	int tmpPrec = 4*std::max<int>( std::max<int>(theta.getPrecision(), phi.getPrecision()), precision );
	theta.setPrecision(tmpPrec);
	phi.setPrecision(tmpPrec);
	
	//clip to 3D and snap to sphere
	m_calc.cartesianFromSpherical(theta, phi, flxs, flys, flzs);
	snap(flxs, flys, flzs, xpq, ypq, zpq, precision, snapType);
	
	xs = Conversion<T_FT>::moveFrom( std::move(xpq) );
	ys = Conversion<T_FT>::moveFrom( std::move(ypq) );
	zs = Conversion<T_FT>::moveFrom( std::move(zpq) );
}


template<typename T_FT>
double ProjectS2::projectFromSpherical(mpfr::mpreal theta, mpfr::mpreal phi, T_FT &xs, T_FT &ys, T_FT &zs, double maxDist, int maxPrecision) const {
	int minSqPrec = std::max<int>(maxPrecision, 256);
	mpfr::mpreal xf, yf, zf, xfs, yfs, zfs, sqd(std::numeric_limits<double>::max());
	{
		if (minSqPrec < std::max<int>(theta.getPrecision(), phi.getPrecision())) {
			m_calc.cartesian(theta, phi, xf, yf, zf);
		}
		else {
			theta.setPrecision(minSqPrec);
			phi.setPrecision(minSqPrec);
			m_calc.cartesian(theta, phi, xf, yf, zf);
		}
	}

	mpfr::mpreal mD2(m_calc.sq(mpfr::mpreal(maxDist)));
	
	//we increment in strides of 64 since gmp uses machine-word for mp_limb_t
	//since this should be used on 64 bit machines we'll use 64 bits here
	for(int prec(64); prec < maxPrecision; prec += 64) {
		projectFromSpherical(theta, phi, xs, ys, zs, prec);
		xfs = Conversion<T_FT>::toMpreal(xs, minSqPrec);
		yfs = Conversion<T_FT>::toMpreal(ys, minSqPrec);
		zfs = Conversion<T_FT>::toMpreal(zs, minSqPrec);
		sqd = m_calc.squaredDistance(m_calc.sub(xfs, xf), m_calc.sub(yfs, yf), m_calc.sub(zfs, zf));
		if (sqd < mD2) {
			break;
		}
	}
	return m_calc.sqrt(sqd).toDouble();
}

template<typename T_FT>
void ProjectS2::toSpherical(const T_FT & xs, const T_FT & ys, const T_FT & zs, double& theta, double& phi, int precision) const {
	mpfr::mpreal xr(Conversion<T_FT>::toMpreal(xs, precision));
	mpfr::mpreal yr(Conversion<T_FT>::toMpreal(ys, precision));
	mpfr::mpreal zr(Conversion<T_FT>::toMpreal(zs, precision));
	
	mpfr::mpreal thetaf, phif;
	m_calc.spherical(xr, yr, zr, thetaf, phif);
	theta = thetaf.toDouble();
	phi = phif.toDouble();
}

template<typename T_FT>
void ProjectS2::toGeo(const T_FT & xs, const T_FT & ys, const T_FT & zs, double & lat, double & lon, int precision) const {
	mpfr::mpreal xr(Conversion<T_FT>::toMpreal(xs, precision));
	mpfr::mpreal yr(Conversion<T_FT>::toMpreal(ys, precision));
	mpfr::mpreal zr(Conversion<T_FT>::toMpreal(zs, precision));
	
	mpfr::mpreal latf, lonf;
	m_calc.geo(xr, yr, zr, latf, lonf);
	lat = latf.toDouble();
	lon = lonf.toDouble();
}

}//end namespace LIB_RATSS_NAMESPACE

#endif