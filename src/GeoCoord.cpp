#include <libratss/GeoCoord.h>

#include <libratss/SphericalCoord.h>
#include <cmath>
#include <ostream>

namespace LIB_RATSS_NAMESPACE {


GeoCoord::GeoCoord() :
lat(0.0),
lon(0.0)
{}

GeoCoord::GeoCoord(const SphericalCoord& other) :
lat( (M_PI/2 - other.theta)/M_PI*2*90 ),
lon( other.phi <= M_PI ? other.phi/M_PI*180 : (other.phi-2*M_PI)/M_PI*180 )
{}

GeoCoord::GeoCoord(const GeoCoord & other) :
lat(other.lat),
lon(other.lon)
{}


GeoCoord::GeoCoord(const std::pair<double, double> & d) :
lat(d.first),
lon(d.second)
{}

GeoCoord::GeoCoord(double _lat, double _lon) :
lat(_lat),
lon(_lon)
{}

GeoCoord & GeoCoord::operator=(const GeoCoord & other) {
	lat = other.lat;
	lon = other.lon;
	return *this;
}

bool GeoCoord::operator!=(const GeoCoord & other) const {
	return lat != other.lat || lon != other.lon;
}

bool GeoCoord::operator==(const GeoCoord & other) const {
	return lat == other.lat && lon == other.lon;
}

bool GeoCoord::operator<(const GeoCoord& other) const {
	return (lat == other.lat ? lon < other.lon : lat < other.lat);
}

std::ostream & operator<<(std::ostream & out, const GeoCoord & gc) {
	out << gc.lat << ' ' << gc.lon;
	return out;
}

std::string to_string(const GeoCoord &sc) {
	using std::to_string;
	return "Geo(" + to_string(sc.lat) + "," + to_string(sc.lon) + ")";
}


}//end namespace LIB_RATSS_NAMESPACE