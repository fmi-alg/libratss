#include <libratss/SphericalCoord.h>

#include <libratss/GeoCoord.h>
#include <cmath>
#include <ostream>
#include <istream>

namespace LIB_RATSS_NAMESPACE {

SphericalCoord::SphericalCoord() :
theta(0.0),
phi(0.0)
{}

SphericalCoord::SphericalCoord(const SphericalCoord & other) :
theta(other.theta),
phi(other.phi)
{}

SphericalCoord::SphericalCoord(double _theta, double _phi) :
theta(_theta),
phi(_phi)
{}

SphericalCoord::SphericalCoord(const std::pair<double, double> & d) :
theta(d.first),
phi(d.second)
{}

SphericalCoord::SphericalCoord(const GeoCoord & other) :
theta( M_PI/180*(90 - other.lat) ),
phi( other.lon >= 0 ? other.lon/180*M_PI : (other.lon + 360)/180*M_PI)
{}

SphericalCoord & SphericalCoord::operator=(const SphericalCoord & other) {
	theta = other.theta;
	phi = other.phi;
	return *this;
}

bool SphericalCoord::operator!=(const SphericalCoord & other) const {
	return theta != other.theta || phi != other.phi;
}

bool SphericalCoord::operator==(const SphericalCoord & other) const {
	return theta == other.theta && phi == other.phi;
}

bool SphericalCoord::operator<(const SphericalCoord & other) const {
	return (theta == other.theta ? phi < other.phi : theta < other.theta);
}

std::ostream & operator<<(std::ostream & out, const SphericalCoord & sc) {
	out << sc.theta << ' ' << sc.phi;
	return out;
}

std::istream & operator<<(std::istream & in, SphericalCoord & sc) {
	in >> sc.theta >> sc.phi;
	return in;
}

std::string to_string(const SphericalCoord &sc) {
	using std::to_string;
	return "Spherical(" + to_string(sc.theta) + "," + to_string(sc.phi) + ")";
}

}//end namespace LIB_RATSS_NAMESPACE