#ifndef LIB_RATSS_GEO_COORD_H
#define LIB_RATSS_GEO_COORD_H
#pragma once

#include <libratss/constants.h>

#include <utility>
#include <iosfwd>
#include <functional>

namespace LIB_RATSS_NAMESPACE {

class SphericalCoord;

class GeoCoord {
public:
	GeoCoord();
	GeoCoord(const GeoCoord & other);
	explicit GeoCoord(const SphericalCoord & other);
	explicit GeoCoord(const std::pair<double, double> & d);
	explicit GeoCoord(double lat, double lon);
	GeoCoord & operator=(const GeoCoord & other);
	bool operator!=(const GeoCoord & other) const;
	bool operator==(const GeoCoord & other) const;
	bool operator<(const GeoCoord & other) const;
public:
	double lat;
	double lon;
};

std::ostream & operator<<(std::ostream & out, const GeoCoord & gc);
std::istream & operator>>(std::istream & in, GeoCoord & gc);

std::string to_string(const GeoCoord & sc);

}//end namespace LIB_RATSS_NAMESPACE

namespace std {

template<>
struct hash< LIB_RATSS_NAMESPACE::GeoCoord > {
	std::hash<double> hS;
	inline void hash_combine(std::size_t & seed, double v) const {
		seed ^= static_cast<std::size_t>(hS(v)) + static_cast<std::size_t>(0x9e3779b9) + (seed << 6) + (seed >> 2);
	}
	inline std::size_t operator()(const LIB_RATSS_NAMESPACE::GeoCoord & v) const {
		std::size_t seed = 0;
		hash_combine(seed, v.lat);
		hash_combine(seed, v.lon);
		return seed;
	}
};

inline std::string to_string(const LIB_RATSS_NAMESPACE::GeoCoord & sc) {
	return LIB_RATSS_NAMESPACE::to_string(sc);
}

} //end namespace std

#endif