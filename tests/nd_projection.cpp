#include <libratss/constants.h>
#include <libratss/ProjectSN.h>

#include "TestBase.h"
#include "../common/generators.h"

namespace LIB_RATSS_NAMESPACE {
namespace tests {

class NDProjectionTest: public TestBase {
CPPUNIT_TEST_SUITE( NDProjectionTest );
// CPPUNIT_TEST( fixPointRandom );
// CPPUNIT_TEST( quadrantTest );
CPPUNIT_TEST( snapFlPlane );
CPPUNIT_TEST( snapFxPlane );
CPPUNIT_TEST( snapCfPlane );
CPPUNIT_TEST( snapJpPlane );
CPPUNIT_TEST( snapFlSphere );
CPPUNIT_TEST( snapFxSphere );
CPPUNIT_TEST( snapCfSphere );
// CPPUNIT_TEST( snapJpSphere );
CPPUNIT_TEST_SUITE_END();
public:
	using Projector = ProjectSN;
public:
	static std::size_t num_random_test_points;
public:
	virtual void setUp() {
		coords = getRandomPolarPoints(num_random_test_points);
	}
public: 
	void quadrantTest();

	void snapFlPlane() { snapRandom({ProjectSN::ST_FL}, {ProjectSN::ST_PLANE}); }
	void snapFxPlane() { snapRandom({ProjectSN::ST_FX}, {ProjectSN::ST_PLANE}); }
	void snapCfPlane() { snapRandom({ProjectSN::ST_CF}, {ProjectSN::ST_PLANE}); }
	void snapJpPlane() { snapRandom({ProjectSN::ST_JP}, {ProjectSN::ST_PLANE}); }
	void snapFlSphere() { snapRandom({ProjectSN::ST_FL}, {ProjectSN::ST_SPHERE}); }
	void snapFxSphere() { snapRandom({ProjectSN::ST_FX}, {ProjectSN::ST_SPHERE}); }
	void snapCfSphere() { snapRandom({ProjectSN::ST_CF}, {ProjectSN::ST_SPHERE}); }
// 	void snaJpSphere() { snapRandom({ProjectSN::ST_JP}, {ProjectSN::ST_SPHERE}); }
protected:
	void snapRandom(const std::vector<int> & snapMethod, const std::vector<int> & snapLocation);
private:
	std::vector<SphericalCoord> coords;
};

std::size_t NDProjectionTest::num_random_test_points;

}} // end namespace ratss::tests

int main(int argc, char ** argv) {
	LIB_RATSS_NAMESPACE::tests::TestBase::init(argc, argv);
	LIB_RATSS_NAMESPACE::tests::NDProjectionTest::num_random_test_points = 10000;
	srand( 0 );
	CppUnit::TextUi::TestRunner runner;
	runner.addTest(  LIB_RATSS_NAMESPACE::tests::NDProjectionTest::suite() );
// 	runner.eventManager().popProtector();
	bool ok = runner.run();
	return ok ? 0 : 1;
}

namespace LIB_RATSS_NAMESPACE {
namespace tests {

void NDProjectionTest::snapRandom(const std::vector<int> & snapMethod, const std::vector<int> & snapLocation) {
	std::array<int, 10> significands = {2, 3, 4, 8, 16, 23, 32, 53, 64, 128};
// 	std::array<int, 5> significands = {23, 32, 53, 64, 128};

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
		//use 2.1 here since an exact 2.0 would only be correct if the reference input point would be the real one (which can't be the case)
		mpq_class projEps = eps*2.1;
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
						ss << "Snapped point with " << sig << "significands and snap-type " << ProjectSN::toString((ProjectSN::SnapType) snapType) << " is too far away: "
							<< Conversion<mpq_class>::toMpreal(dist/eps, 53) << '=' << dist<< "eps !< " << projEps << '\n';
						ss << "P(";
						RationalPoint(inputRational.begin(), inputRational.end()).print(ss, RationalPoint::FM_FLOAT128);
						ss << ") -> ";
						RationalPoint(output.begin(), output.end()).print(ss, RationalPoint::FM_FLOAT128);
						CPPUNIT_ASSERT_MESSAGE(ss.str(), dist <= projEps);
					}
				}
			}
		}
	}
}

void NDProjectionTest::quadrantTest() {
	std::vector<GeoCoord> myCoords;
	Projector p;
	GeoCalc gc;
	
	for(double lat(15); lat > -166; lat -= 15) {
		for(double lon(0); lon < 360; lon += 15) {
			myCoords.emplace_back(lat, lon);
		}
	}
	for(uint32_t bits(32); bits < 128; bits += 16) {
		for(const GeoCoord & coord : myCoords) {
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