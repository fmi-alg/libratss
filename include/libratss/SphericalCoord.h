#ifndef LIB_RATSS_SPHERICAL_COORD_H
#define LIB_RATSS_SPHERICAL_COORD_H
#pragma once

#include <libratss/constants.h>

#include <utility>
#include <iosfwd>
#include <functional>

namespace LIB_RATSS_NAMESPACE {

class GeoCoord;

class SphericalCoord {
public:
	SphericalCoord();
	SphericalCoord(const SphericalCoord & other);
	explicit SphericalCoord(double theta, double phi);
	explicit SphericalCoord(const std::pair<double, double> & d);
	explicit SphericalCoord(const GeoCoord & other);
	SphericalCoord & operator=(const SphericalCoord & other);
	bool operator!=(const SphericalCoord & other) const;
	bool operator==(const SphericalCoord & other) const;
	bool operator<(const SphericalCoord & other) const;
public:
	double theta;
	double phi;
};

std::ostream & operator<<(std::ostream & out, const SphericalCoord & sc);
std::istream & operator>>(std::istream & in, SphericalCoord & sc);

std::string to_string(const SphericalCoord & sc);

}//end namespace LIB_RATSS_NAMESPACE

namespace std {

template<>
struct hash< LIB_RATSS_NAMESPACE::SphericalCoord > {
	std::hash<double> hS;
	inline void hash_combine(uint64_t & seed, double v) const {
		seed ^= static_cast<uint64_t>(hS(v)) + static_cast<uint64_t>(0x9e3779b9) + (seed << 6) + (seed >> 2);
	}
	inline size_t operator()(const LIB_RATSS_NAMESPACE::SphericalCoord & v) const {
		uint64_t seed = 0;
		hash_combine(seed, v.theta);
		hash_combine(seed, v.phi);
		return seed;
	}
};

inline string to_string(const LIB_RATSS_NAMESPACE::SphericalCoord & sc) {
	return LIB_RATSS_NAMESPACE::to_string(sc);
}

} //end namespace std

#endif