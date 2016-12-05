#include <libratss/constants.h>
#include <libratss/Projector.h>

#include "TestBase.h"
#include <common/generators.h>

namespace LIB_RATSS_NAMESPACE {
namespace tests {

class ProjectionTest: public TestBase {
CPPUNIT_TEST_SUITE( ProjectionTest );
CPPUNIT_TEST( fixPointRandom );
CPPUNIT_TEST( bijectionSpecial );
CPPUNIT_TEST( bijectionSpecial2 );
CPPUNIT_TEST( quadrantTest );
CPPUNIT_TEST_SUITE_END();
public:
	static std::size_t num_random_test_points;
public:
	void fixPointRandom();
	void bijectionSpecial();
	void bijectionSpecial2();
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
	std::vector<SphericalCoord> coords = getRandomPolarPoints(num_random_test_points);
	
	Projector p;
	
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
		
		CPPUNIT_ASSERT_EQUAL(coord.theta, theta);
		CPPUNIT_ASSERT_EQUAL(coord.phi, phi);
	}
}

void ProjectionTest::bijectionSpecial() {
	Projector p;
	
	
	SphericalCoord sp{1.5707963267948966l, 0.73303828583761843l};
	GeoCoord gc(sp);
	
	CORE::Expr xc, yc, zc;
	
	
	mpq_class xs, ys, zs;
	
	mpq_class sp_x("1595891361/2147483648");
	mpq_class sp_y("2873894071/4294967296");
	
	CPPUNIT_ASSERT(sp_x*sp_x + sp_y*sp_y <= 1);
	
	
	//reproject onto sphere
// 	p.stInverseProject(sp_x, sp_y, RationalSphere::Projector::SP_LOWER, xs, ys, zs);
	{
		using comp_class = mpq_class;
		
		comp_class zero(0);
		comp_class one(1);
		comp_class two(2);
		comp_class minusOne(-1);
	
		comp_class xp(sp_x);
		comp_class yp(sp_y);
		comp_class denom = one + xp*xp + yp*yp;
		comp_class nomZ = minusOne + xp*xp + yp*yp;
		xs = (two*xp) / denom;
		ys = (two*yp) / denom;
		zs = (nomZ / denom);

// 		std::cout << "denom=" << denom << '\n';
// 		std::cout << "nom=" << nomZ << '\n';
// 		std::cout << "xs=" << ys << '\n';
// 		std::cout << "ys=" << ys << '\n';
// 		std::cout << "zs=" << zs << '\n';
		
		zs *= -1;
// 		std::cout << "-zs=" << zs << '\n';
		
		comp_class xp_1, yp_1;
		
		if (zs <= zero) {
			mpq_class ztmp(zs);
			ztmp *= -1;
			xp_1 = xs / (1-ztmp); 
			yp_1 = ys / (1-ztmp);
		}
		else {
			xp_1 = xs / (1+zs); 
			yp_1 = ys / (1+zs);
		}
// 		std::cout << "xp_1=" << xp_1 << '\n';
// 		std::cout << "yp_1=" << yp_1 << '\n';
		
		CPPUNIT_ASSERT_EQUAL(xp_1, sp_x);
		CPPUNIT_ASSERT_EQUAL(yp_1, sp_y);
	}
	
	p.projectFromSpherical(sp.theta, sp.phi, xc, yc, zc, 32);

	mpq_class sqlen = xs*xs + ys*ys + zs*zs;
	CPPUNIT_ASSERT_EQUAL_MESSAGE("Projected point is not on the sphere", mpq_class(1), sqlen);
	
	{
		mpq_class sp_x2, sp_y2;
		p.stProject(xs, ys, zs, sp_x2, sp_y2);
		CPPUNIT_ASSERT_EQUAL(sp_x, sp_x2);
		CPPUNIT_ASSERT_EQUAL(sp_y, sp_y2);
	}
}

void ProjectionTest::bijectionSpecial2() {
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

	Projector proc;
	for(const auto & coord : coords) {
		double lat = coord.lat;
		double lon = coord.lon;
		mpfr::mpreal ilatf(lat, 1024);
		mpfr::mpreal ilonf(lon, 1024);
		mpfr::mpreal xf, yf, zf, latf, lonf;
		CORE::Expr xs, ys, zs;
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
	Projector p;
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