#include <libratss/constants.h>
#include <libratss/ProjectSN.h>

#include "TestBase.h"
#include "../common/generators.h"
#include "../common/points.h"

namespace LIB_RATSS_NAMESPACE {
namespace tests {

class NDProjectionTest: public TestBase {
CPPUNIT_TEST_SUITE( NDProjectionTest );
CPPUNIT_TEST( fixPointRandom );
CPPUNIT_TEST( snapRandom );
CPPUNIT_TEST( quadrantTest );
CPPUNIT_TEST_SUITE_END();
public:
	using Projector = ProjectSN;
public:
	static std::size_t num_random_test_points;
public:
	void fixPointRandom();
	void snapRandom();
	void bijectionSpecial();
	void bijectionSpecial2();
	void quadrantTest();
};

std::size_t NDProjectionTest::num_random_test_points;

}} // end namespace ratss::tests

int main(int argc, char ** argv) {
	LIB_RATSS_NAMESPACE::tests::TestBase::init(argc, argv);
	LIB_RATSS_NAMESPACE::tests::NDProjectionTest::num_random_test_points = 10000;
	srand( 0 );
	CppUnit::TextUi::TestRunner runner;
	runner.addTest(  LIB_RATSS_NAMESPACE::tests::NDProjectionTest::suite() );
	bool ok = runner.run();
	return ok ? 0 : 1;
}

namespace LIB_RATSS_NAMESPACE {
namespace tests {

void NDProjectionTest::fixPointRandom() {
	std::vector<SphericalCoord> coords = getRandomPolarPoints(num_random_test_points);
	
	Projector p;
	GeoCalc gc;
	
	for(uint32_t prec(16); prec < 128; prec += 16) {
		for(const SphericalCoord & coord : coords) {
			mpfr::mpreal xd, yd, zd;
			gc.cartesianFromSpherical(coord.theta, coord.phi, xd ,yd, zd);
			
			CPPUNIT_ASSERT_MESSAGE("Projection to cartesian coordinates is not precise enough", (xd*xd+yd*yd+zd*zd) <= 1.1);
			
			mpq_class input[3] = {
				Conversion<mpfr::mpreal>::toMpq(xd),
				Conversion<mpfr::mpreal>::toMpq(yd),
				Conversion<mpfr::mpreal>::toMpq(zd)
			};
			mpq_class output[3];
			auto pos = p.sphere2Plane(input, input+3, output);
			
			CPPUNIT_ASSERT(std::abs<int>(pos) >= 1 && std::abs<int>(pos) <= 3);
			CPPUNIT_ASSERT_MESSAGE("Projection to (d-1) dimensions is not in circle", (output[0]*output[0] + output[1]*output[1]+output[2]*output[2]) <= mpq_class(1));
			CPPUNIT_ASSERT_EQUAL_MESSAGE("Projection coordinate is not zero", mpq_class(0), output[abs((int)pos)-1]);
			
			p.plane2Sphere(output, output+3, pos, input);
			mpq_class & x = input[0];
			mpq_class & y = input[1];
			mpq_class & z = input[2];
			std::stringstream ss;
			ss << "Point " << to_string(coord) << " does not project on sphere for precision=" << prec;
			mpq_class sqlen = x*x+y*y+z*z;
			CPPUNIT_ASSERT_EQUAL_MESSAGE(ss.str(), mpq_class(1), sqlen);
		}
	}
}

void NDProjectionTest::snapRandom() {
	std::array<int, 10> significands = {2, 3, 4, 8, 16, 23, 32, 53, 64, 128};
// 	std::array<int, 5> significands = {23, 32, 53, 64, 128};
	std::array<int, 3> snapMethod = {ProjectSN::ST_FX, ProjectSN::ST_FL, ProjectSN::ST_CF};
// 	std::array<int, 2> snapMethod = {ProjectSN::ST_FX, ProjectSN::ST_FL};
	std::array<int, 2> snapLocation = {ProjectSN::ST_PLANE, ProjectSN::ST_SPHERE };

// 	std::array<int, 1> snapMethod = {ProjectSN::ST_CF};
// 	std::array<int, 1> snapLocation = {ProjectSN::ST_SPHERE};
	
	std::vector<SphericalCoord> coords = getRandomPolarPoints(num_random_test_points);
	
	Projector p;
	GeoCalc gc;
	
	std::array<mpfr::mpreal, 3> input;
	std::array<mpq_class, 3> inputRational;
	std::array<mpq_class, 3> output;
	
	for(int sig : significands) {
		int prec = std::max<int>(53, 2*sig);
		mpq_class eps(mpz_class(1), mpz_class(1) << sig);
		mpq_class eps2 = eps*eps;
		mpq_class maxSqLen = 1+6*eps+3*eps2;
		mpq_class projEps = eps*3;
		for(const SphericalCoord & sc : coords) {
			gc.cartesianFromSpherical(mpfr::mpreal(sc.theta, prec), mpfr::mpreal(sc.phi, prec), input[0], input[1], input[2]);
			std::transform(input.begin(), input.end(), inputRational.begin(), [](const mpfr::mpreal & x) {
				return Conversion<mpfr::mpreal>::toMpq(x);
			});
			
			{
				mpq_class sqLenInput(0);
				for(const mpq_class & x : inputRational) {
					sqLenInput += x*x;
				}
				std::stringstream ss;
				ss << "Input points are too far away: (squared-length - 1) =";
				ss << Conversion<mpq_class>::toMpreal(sqLenInput, 128);
				ss << " ~ " << Conversion<mpq_class>::toMpreal((sqLenInput-1)/eps, 53) << "eps";
				CPPUNIT_ASSERT_MESSAGE(ss.str(), sqLenInput < maxSqLen);
			}

			for(int sm : snapMethod) {
				for (int sl : snapLocation) {
					int snapType = sm | sl;
					
					p.snap(input.begin(), input.end(), output.begin(), snapType, sig);
					
					mpq_class sqLen = output[0]*output[0] + output[1]*output[1] + output[2]*output[2];
					CPPUNIT_ASSERT_EQUAL_MESSAGE("Snapped point is not on the sphere", mpq_class(1), sqLen);
					
					for(int i(0); i < 3; ++i) {
						using std::abs;
						mpq_class dist = abs(inputRational[i]-output[i]);
						std::stringstream ss;
						ss << "Snap point with significands " << sig << " and snap-type " << snapType << " is too far away: "
							<< dist << "=" << Conversion<mpq_class>::toMpreal(dist/eps, 53) << "eps !< " << projEps << '\n';
						ss << "P(";
						OutputPoint(inputRational.begin(), inputRational.end()).print(ss, OutputPoint::FM_FLOAT128);
						ss << ") -> ";
						OutputPoint(output.begin(), output.end()).print(ss, OutputPoint::FM_FLOAT128);
						CPPUNIT_ASSERT_MESSAGE(ss.str(), dist <= projEps);
					}
				}
			}
		}
	}
}

void NDProjectionTest::quadrantTest() {
	std::vector<GeoCoord> coords;
	Projector p;
	GeoCalc gc;
	
	for(double lat(15); lat > -166; lat -= 15) {
		for(double lon(0); lon < 360; lon += 15) {
			coords.emplace_back(lat, lon);
		}
	}
	for(uint32_t bits(32); bits < 128; bits += 16) {
		for(const GeoCoord & coord : coords) {
			mpfr::mpreal xd, yd, zd;
			gc.cartesian(coord.lat, coord.lon, xd ,yd, zd);
			
			CPPUNIT_ASSERT_MESSAGE("Projection to cartesian coordinates is not precise enough", (xd*xd+yd*yd+zd*zd) <= 1.1);
			
			mpq_class points3d[3] = {
				Conversion<mpfr::mpreal>::toMpq(xd),
				Conversion<mpfr::mpreal>::toMpq(yd),
				Conversion<mpfr::mpreal>::toMpq(zd)
			};
			mpq_class points2d[3];
			auto pos = p.sphere2Plane(points3d, points3d+3, points2d);
			
			CPPUNIT_ASSERT(std::abs<int>(pos) >= 1 && std::abs<int>(pos) <= 3);
			CPPUNIT_ASSERT_EQUAL_MESSAGE("Projection coordinate is not zero", mpq_class(0), points2d[abs((int)pos)-1]);
			CPPUNIT_ASSERT_MESSAGE("Projection to (d-1) dimensions is not in circle", (points2d[0]*points2d[0] + points2d[1]*points2d[1]+points2d[2]*points2d[2]) <= mpq_class(1));
			
			p.plane2Sphere(points2d, points2d+3, pos, points3d);
			mpq_class & xs = points3d[0];
			mpq_class & ys = points3d[1];
			mpq_class & zs = points3d[2];
			std::stringstream ss;
			ss << "Projection of coordinate " << coord << " is not on the sphere with precision=" << bits;
			mpq_class sqlen = xs*xs + ys*ys + zs*zs;
			CPPUNIT_ASSERT_EQUAL_MESSAGE(ss.str(), mpq_class(1), sqlen);
		}
	}
}

}} //end namespace LIB_RATSS_NAMESPACE::tests