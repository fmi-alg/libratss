#include <libratss/constants.h>
#include <libratss/ProjectS2.h>

#include "TestBase.h"
#include "../common/generators.h"

#include <thread>

namespace LIB_RATSS_NAMESPACE {
namespace tests {
namespace {
	static std::size_t num_random_test_points = 10000;
}

#define CLS_TMPL_DECL template<int snapPosition, int snapType>
#define CLS_TMPL_NAME ProjectionTest<snapPosition,snapType>

CLS_TMPL_DECL
class ProjectionTest: public TestBase {
CPPUNIT_TEST_SUITE( ProjectionTest );
CPPUNIT_TEST( fixPointRandom );
CPPUNIT_TEST( bijectionSpecial );
CPPUNIT_TEST( quadrantTest );
CPPUNIT_TEST_SUITE_END();
public:
	void fixPointRandom();
	void bijectionSpecial();
	void quadrantTest();
};

}} // end namespace ratss::tests

int main(int argc, char ** argv) {
	using namespace LIB_RATSS_NAMESPACE;
	using namespace LIB_RATSS_NAMESPACE::tests;
	TestBase::init(argc, argv);
	srand( 0 );
	
	uint32_t numThreads = 1;
	for(int i(1); i < argc; ++i) {
		std::string token(argv[i]);
		if (token == "--threads" && i+1 < argc) {
			numThreads = std::atoi(argv[i+1]);
			++i;
		}
		else if (token == "--help" || token == "-h") {
			std::cerr << "prg [--threads <number>]" << std::endl;
			return 0;
		}
	}
	std::vector<std::unique_ptr<CppUnit::TextUi::TestRunner>> runners;

#define TEST_INSTANCE(__SNAP_POSITION, __SNAP_TYPE) \
	do { \
		runners.push_back(std::make_unique<CppUnit::TextUi::TestRunner>()); \
		runners.back()->addTest(  ProjectionTest<__SNAP_POSITION, __SNAP_TYPE>::suite() ); \
	} while (0);
	
	TEST_INSTANCE(ST_PLANE, ST_CF);
	TEST_INSTANCE(ST_PLANE, ST_FX);
	TEST_INSTANCE(ST_PLANE, ST_FL);
	TEST_INSTANCE(ST_PLANE, ST_JP);
	
	TEST_INSTANCE(ST_SPHERE, ST_CF);
	TEST_INSTANCE(ST_SPHERE, ST_FL);
	TEST_INSTANCE(ST_SPHERE, ST_FX);
	
#if defined(LIB_RATSS_WITH_CGAL)
	TEST_INSTANCE(ST_PAPER, ST_CF);
	TEST_INSTANCE(ST_PAPER, ST_FX);
	TEST_INSTANCE(ST_PAPER, ST_JP);
#endif

#if defined(LIB_RATSS_WITH_FPLLL)
	TEST_INSTANCE(ST_PLANE, ST_FPLLL);
	TEST_INSTANCE(ST_SPHERE, ST_FPLLL);
#endif
	
#if defined(LIB_RATSS_WITH_CGAL) and defined(LIB_RATSS_WITH_FPLLL)
	TEST_INSTANCE(ST_PAPER, ST_FPLLL);
#endif
	
#undef TEST_INSTANCE
	
	std::vector<std::thread> threads;
	std::atomic<std::size_t> runnerId{0};
	std::atomic<bool> ok{true};
	for(std::size_t i(0); i < numThreads; ++i) {
		threads.emplace_back([&]() {
			while(true) {
				std::size_t i = runnerId.fetch_add(1, std::memory_order_relaxed);
				if (i < runners.size()) {
					if (!runners[i]->run()) {
						ok = false;
					}
				}
				else {
					break;
				}
			}
		});
	}
	
	for(auto & x : threads) {
		x.join();
	}
	
	return ok ? 0 : 1;
}

namespace LIB_RATSS_NAMESPACE {
namespace tests {
	

CLS_TMPL_DECL
void CLS_TMPL_NAME::fixPointRandom() {
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
			p.projectFromSpherical(coord.theta, coord.phi, x, y, z, int(prec), snapPosition | snapType);
			std::stringstream ss;
			ss << "Point " << to_string(coord) << " does not project on sphere for precision=" << prec;
			mpq_class sqlen = x*x+y*y+z*z;
			CPPUNIT_ASSERT_EQUAL_MESSAGE(ss.str(), mpq_class(1), sqlen);
		}
	}
	
	for(const SphericalCoord & coord : coords) {
		mpq_class x,y,z;
		p.projectFromSpherical(coord.theta, coord.phi, x, y, z, int(128), snapPosition | snapType);
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

CLS_TMPL_DECL
void CLS_TMPL_NAME::bijectionSpecial() {
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
		proc.projectFromGeo(lat, lon, xs, ys, zs, 128, snapPosition | snapType);
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

CLS_TMPL_DECL
void CLS_TMPL_NAME::quadrantTest() {
	std::vector<GeoCoord> coords;
	std::vector<std::array<mpq_class, 3>> cartesians;
	ProjectS2 p;
	for(double lat(15); lat > -166; lat -= 15) {
		for(double lon(0); lon < 360; lon += 15) {
			coords.emplace_back(lat, lon);
			std::array<mpq_class, 3> cartesian;
			p.projectFromGeo(coords.back().lat, coords.back().lon, cartesian[0], cartesian[1], cartesian[2], 512, ST_SPHERE | ST_FL | ST_NORMALIZE);
			cartesians.push_back(cartesian);
		}
	}
	
	std::array<mpq_class, 3> point;

	for(int bits(32); bits < 128; bits += 16) {
		//points are not exactly on the sphere and we compare the result with an inexact computation
		//We therefore have to leave some wiggle room (except in the paper snapping case
		mpq_class eps;
		if (snapPosition == ST_PAPER) {
			eps = mpq_class(mpz_class(1), mpz_class(1) << (bits-1));
		}
		else {
			eps = mpq_class(mpz_class(1), mpz_class(1) << (bits-2));
		}
		for(std::size_t i(0); i < coords.size(); ++i) {
			const GeoCoord & coord  = coords[i];
			std::stringstream ss;
			ss << "Coordinate " << i << "=(lat,lon)=(" << coord.lat << ", " << coord.lon << "); ";
			ss << "bits=" << bits << "; ";
			ss << "snapType=" << ProjectSN::toString(ProjectSN::SnapType(snapType)) << "; ";
			ss << "snapPosition=" << ProjectSN::toString(ProjectSN::SnapType(snapPosition));
			std::string errmsg = ss.str();
			CPPUNIT_ASSERT_NO_THROW_MESSAGE(
				errmsg,
				p.projectFromGeo(coord.lat, coord.lon, point[0], point[1], point[2], bits, snapType | snapPosition | ST_NORMALIZE)
			);
			
			mpq_class sqlen = point[0]*point[0] + point[1]*point[1] + point[2]*point[2];
			CPPUNIT_ASSERT_EQUAL_MESSAGE(ss.str(), mpq_class(1), sqlen);
			
			for(std::size_t j(0); j < point.size(); ++j) {
				using std::abs;
				std::stringstream ss;
				ss << errmsg << "; p[" << j << "]=" << point[j] << " : " << "bits(p[" << j << "])=" << numBits(point[j]) <<  " > " << std::size_t(2*bits);
				CPPUNIT_ASSERT_MESSAGE(ss.str(), numBits(point[j]) <= std::size_t(2*bits));
				ss.str(errmsg);
				mpq_class dist = abs(cartesians[i][j]-point[j]);
				ss << ": " << "abs(p[" << j << "]=" << point[j] << " - real)=";
				ss << dist << " > eps=";
				ss << eps;
				CPPUNIT_ASSERT_MESSAGE(ss.str(), dist <= eps);
			}
		}
	}
}

}} //end namespace LIB_RATSS_NAMESPACE::tests

#undef CLS_TMPL_DECL
#undef CLS_TMPL_NAME
