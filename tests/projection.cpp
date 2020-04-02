#include <libratss/constants.h>
#include <libratss/ProjectS2.h>

#include "TestBase.h"
#include "../common/generators.h"

namespace LIB_RATSS_NAMESPACE {
namespace tests {

class ProjectionTest: public TestBase {
CPPUNIT_TEST_SUITE( ProjectionTest );
CPPUNIT_TEST( fixPointRandom );
CPPUNIT_TEST( bijectionSpecial );
CPPUNIT_TEST( quadrantTest );
CPPUNIT_TEST_SUITE_END();
public:
	static std::size_t num_random_test_points;
public:
	void fixPointRandom();
	void bijectionSpecial();
	void quadrantTest();
};

std::size_t ProjectionTest::num_random_test_points;

}} // end namespace ratss::tests

int main(int argc, char ** argv) {
	LIB_RATSS_NAMESPACE::tests::TestBase::init(argc, argv);
	LIB_RATSS_NAMESPACE::tests::ProjectionTest::num_random_test_points = 10000;
	srand( 0 );
	CppUnit::TextUi::TestRunner runner;
	runner.addTest(  LIB_RATSS_NAMESPACE::tests::ProjectionTest::suite() );
	bool ok = runner.run();
	return ok ? 0 : 1;
}

namespace LIB_RATSS_NAMESPACE {
namespace tests {

void ProjectionTest::fixPointRandom() {
	std::vector<SphericalCoord> coords;
#if defined(LIB_RATSS_WITH_CGAL)
	getRandomPolarPoints(num_random_test_points, std::back_inserter(coords));
#else
	{
		auto tmp = getRandomGeoPoints(num_random_test_points, Bounds(-90, 90, -180, 190));
		for(auto const & x : tmp) {
			coords.push_back(SphericalCoord(x));
		}
	}
#endif
	ProjectS2 p;
	
	for(uint32_t prec(16); prec < 128; prec += 16) {
		for(const SphericalCoord & coord : coords) {
			mpq_class x,y,z;
			p.projectFromSpherical(coord.theta, coord.phi, x, y, z, prec);
			std::stringstream ss;
			ss << "Point " << to_string(coord) << " does not project on sphere for precision=" << prec;
			mpq_class sqlen = x*x+y*y+z*z;
			CPPUNIT_ASSERT_EQUAL_MESSAGE(ss.str(), mpq_class(1), sqlen);
		}
	}
	
	for(const SphericalCoord & coord : coords) {
		mpq_class x,y,z;
		p.projectFromSpherical(coord.theta, coord.phi, x, y, z, 128);
		std::stringstream ss;
		ss << "Point " << to_string(coord) << " does not project on sphere for precision=" << 128;
		mpq_class sqlen = x*x+y*y+z*z;
		CPPUNIT_ASSERT_EQUAL_MESSAGE(ss.str(), mpq_class(1), sqlen);
		
		double theta, phi;
		p.toSpherical(x, y, z, theta, phi, 128);
		
		CPPUNIT_ASSERT_EQUAL_MESSAGE("theta missmatch after reprojection", coord.theta, theta);
		CPPUNIT_ASSERT_EQUAL_MESSAGE("phi missmatch after reprojection", coord.phi, phi);
	}
}

void ProjectionTest::bijectionSpecial() {
	auto upperCoords = getRandomGeoPoints(1024, Bounds(30, 40, 5, 15));
	auto lowerCoords = getRandomGeoPoints(1024, Bounds(-40, -30, 5, 15));
	
	decltype(upperCoords) coords;
	coords.emplace_back(-50, 10);
	coords.emplace_back(50, 10);
	coords.emplace_back(40, 10);
	coords.emplace_back(40-90, 10);
	coords.emplace_back(40, 10-90);
	coords.emplace_back(40-90, 10-180);
	coords.insert(coords.end(), upperCoords.begin(), upperCoords.end());

	coords.insert(coords.end(), lowerCoords.begin(), lowerCoords.end());

	ProjectS2 proc;
	for(const auto & coord : coords) {
		double lat = coord.lat;
		double lon = coord.lon;
		mpfr::mpreal ilatf(lat, 1024);
		mpfr::mpreal ilonf(lon, 1024);
		mpfr::mpreal xf, yf, zf, latf, lonf;

		mpq_class xs, ys, zs;
		double latOut, lonOut;
		
		proc.calc().cartesian(ilatf, ilonf, xf, yf, zf);
		proc.calc().geo(xf, yf, zf, latf, lonf);
		proc.projectFromGeo(lat, lon, xs, ys, zs, 128);
		proc.toGeo(xs, ys, zs, latOut, lonOut, 128);

		double xfd(xf.toDouble());
		double yfd(yf.toDouble());
		double zfd(zf.toDouble());
		double latfd = latf.toDouble();
		double lonfd = lonf.toDouble();
		
		CPPUNIT_ASSERT_EQUAL(lat, latOut);
		CPPUNIT_ASSERT_EQUAL(lon, lonOut);
		CPPUNIT_ASSERT_EQUAL(lat, latfd);
		CPPUNIT_ASSERT_EQUAL(lon, lonfd);
	}
}

void ProjectionTest::quadrantTest() {
	std::vector<GeoCoord> coords;
	ProjectS2 p;
	for(double lat(15); lat > -166; lat -= 15) {
		for(double lon(0); lon < 360; lon += 15) {
			coords.emplace_back(lat, lon);
		}
	}
	mpq_class xs, ys, zs;
	for(uint32_t bits(32); bits < 128; bits += 16) {
		for(const GeoCoord & coord : coords) {
			p.projectFromGeo(coord.lat, coord.lon, xs, ys, zs, bits);
			std::stringstream ss;
			ss << "Projection of coordinate " << coord << " is no on the sphere";
			mpq_class sqlen = xs*xs + ys*ys + zs*zs;
			CPPUNIT_ASSERT_EQUAL_MESSAGE(ss.str(), mpq_class(1), sqlen);
		}
	}
}

}} //end namespace LIB_RATSS_NAMESPACE::tests