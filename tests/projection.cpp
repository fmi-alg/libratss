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
// CPPUNIT_TEST( fixPointRandom );
// CPPUNIT_TEST( bijectionSpecial );
CPPUNIT_TEST( quadrantTest );
CPPUNIT_TEST_SUITE_END();
public:
	void fixPointRandom();
	void bijectionSpecial();
	void quadrantTest();
};


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
			p.projectFromSpherical(mpfr::mpreal(coord.theta), mpfr::mpreal(coord.phi), x, y, z, int(prec), snapPosition | snapType);
			std::stringstream ss;
			ss << "Point " << to_string(coord) << " does not project on sphere for precision=" << prec;
			mpq_class sqlen = x*x+y*y+z*z;
			CPPUNIT_ASSERT_EQUAL_MESSAGE(ss.str(), mpq_class(1), sqlen);
		}
	}
	
	for(const SphericalCoord & coord : coords) {
		mpq_class x,y,z;
		p.projectFromSpherical(mpfr::mpreal(coord.theta), mpfr::mpreal(coord.phi), x, y, z, int(128), snapPosition | snapType);
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
		proc.projectFromGeo(mpfr::mpreal(lat), mpfr::mpreal(lon), xs, ys, zs, 128, snapPosition | snapType);
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
			p.projectFromGeo(mpfr::mpreal(coords.back().lat), mpfr::mpreal(coords.back().lon), cartesian[0], cartesian[1], cartesian[2], 1024, ST_PLANE | ST_FL | ST_NORMALIZE);
			cartesians.push_back(cartesian);
		}
	}
	
	std::array<mpq_class, 3> point;
	
	for(int bits(32); bits < 128; bits += 16) {
		mpq_class eps = mpq_class(mpz_class(1), mpz_class(1) << bits);
		std::size_t maxQ{0}; //number of bits the lcm of the denominators of coordinates in the plane, this is the Q in the paper
		//In the following eps=2^-significands
		//For ST_CF we need one bit more since we first have to compute a rational with at least eps/4 close to the correct one
		//from there we compute a eps/2 approximation to that value thus guaranteeing a 3/4*eps apx
		//For ST_JP we do the same but may not find a better sim-apx thus returning the eps/4 apx used with ST_FX thus 2 bits more
		//
		//In the following we have significands=n
		//For ST_FX and ST_SPHERE we have as input to the projection
		//x_i = p_i/2^n with the pole coordinate x_d = p_d/2^n
		//In the plane we have
		//y_i = x_i/(1-x_d) = (p_i/2^n)/(1-p_d/2^n) = (p_i/2^n)* 2^n/(2^n-p_d) = p_i/(2^n-p_d)
		//Thus an lcm of (2^n-p_d)=Q thus Q<=2^(n+1) since p_d may be negative but abs(p_d) <= 2^n
		switch (snapType) {
		case ST_CF_GUARANTEE_SIZE:
			maxQ = bits;
			break;
		case ST_CF_GUARANTEE_DISTANCE:
			maxQ = bits+2;
			break;
		case ST_FPLLL_GUARANTEE_SIZE:
			maxQ = bits;
			break;
		case ST_FPLLL_GUARANTEE_DISTANCE:
			maxQ = bits*point.size();
			break;
		case ST_FX:
			maxQ = bits;
			if (snapPosition & ST_SPHERE) {
				maxQ += 1;
			}
			break;
		case ST_JP_GUARANTEE_SIZE:
			maxQ = bits;
			break;
		case ST_JP_GUARANTEE_DISTANCE:
			maxQ = bits+2;
			break;
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
				p.projectFromGeo(mpfr::mpreal(coord.lat), mpfr::mpreal(coord.lon), point[0], point[1], point[2], bits, snapType | snapPosition | ST_NORMALIZE)
			);
			
			mpq_class sqlen = point[0]*point[0] + point[1]*point[1] + point[2]*point[2];
			CPPUNIT_ASSERT_EQUAL_MESSAGE(ss.str(), mpq_class(1), sqlen);
			
			for(std::size_t j(0); j < point.size(); ++j) {
				using std::abs;
				std::stringstream ss;
				//ST_FL may result in bit sizes larger than the requested number of bits due to the exponent
				if (snapType & (ST_FX | ST_GUARANTEE_SIZE)) {
					ss << errmsg << "; p[" << j << "]=" << point[j] << " : ";
					ss << "bits(p[" << j << "])=" << numBits(point[j].get_num()) << "/" << numBits(point[j].get_den());
					ss <<  " > " << std::size_t(2*bits+1) << "/" << std::size_t(2*bits+2);
					//algo guarantees
					//num <= Q^2 for j = posOnSphere(...)
					//num <= 2*Q^2 for j != posOnSphere(...)
					//with Q=2^bits -> num <= 2^(2*bits) -> we need 2*bits+1 bits to encode the number 2^(2*bits)
					int pos = p.positionOnSphere(point.begin(), point.end()); //position on sphere is shifted by 1
					if (abs(pos)-1 == int(j)) {
						CPPUNIT_ASSERT_MESSAGE(ss.str(), numBits(point[j].get_num()) <= std::size_t(2*maxQ+1));
					}
					else {
						CPPUNIT_ASSERT_MESSAGE(ss.str(), numBits(point[j].get_num()) <= std::size_t(2*maxQ+2));
					}
					//algo guarantees den <= 2*Q^2 with Q=2^bits -> num <= 2*2^(2*bits) -> we need 2*bits+2 bits to encode the number 2^(2*bits+1)
					CPPUNIT_ASSERT_MESSAGE(ss.str(), numBits(point[j].get_den()) <= std::size_t(2*maxQ+2));
				}
				if (snapType & (ST_FL | ST_FX | ST_GUARANTEE_DISTANCE)) {
					ss = std::stringstream();
					mpq_class dist = abs(cartesians[i][j]-point[j]);
					mpq_class distInEps = dist/eps;
					ss << errmsg << ": " << "abs(p[" << j << "]=" << point[j] << "~" << point[j].get_d() << " - real~" << cartesians[i][j].get_d() << ")=";
					ss << dist << "~" << dist.get_d() << "~" << distInEps.get_d() << "[eps] > eps=";
					ss << eps << "~" << eps.get_d();
					//points are not exactly on the sphere and we compare the result with an inexact computation
					//We therefore have to leave some wiggle room (except in the paper snapping case
					CPPUNIT_ASSERT_MESSAGE(ss.str(), mpq_class(dist/eps).get_d() <= 2.01);
				}
			}
		}
	}
}

#undef CLS_TMPL_DECL
#undef CLS_TMPL_NAME

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
	
	TEST_INSTANCE(ST_PLANE, ST_CF_GUARANTEE_DISTANCE);
	TEST_INSTANCE(ST_PLANE, ST_CF_GUARANTEE_SIZE);
	TEST_INSTANCE(ST_PLANE, ST_FX);
	TEST_INSTANCE(ST_PLANE, ST_FL);
	TEST_INSTANCE(ST_PLANE, ST_JP_GUARANTEE_DISTANCE);
	TEST_INSTANCE(ST_PLANE, ST_JP_GUARANTEE_SIZE);
	
	TEST_INSTANCE(ST_SPHERE, ST_CF_GUARANTEE_DISTANCE);
	TEST_INSTANCE(ST_SPHERE, ST_CF_GUARANTEE_SIZE);
	TEST_INSTANCE(ST_SPHERE, ST_FL);
	TEST_INSTANCE(ST_SPHERE, ST_FX);
	
#if defined(LIB_RATSS_WITH_CGAL)
	TEST_INSTANCE(ST_PAPER, ST_CF_GUARANTEE_DISTANCE);
	TEST_INSTANCE(ST_PAPER, ST_CF_GUARANTEE_SIZE);
	TEST_INSTANCE(ST_PAPER, ST_FX);
	TEST_INSTANCE(ST_PAPER, ST_JP_GUARANTEE_DISTANCE);
	TEST_INSTANCE(ST_PAPER, ST_JP_GUARANTEE_SIZE);
#endif
	
#if defined(LIB_RATSS_WITH_CORE_TWO)
	TEST_INSTANCE(ST_PAPER2, ST_CF_GUARANTEE_DISTANCE);
	TEST_INSTANCE(ST_PAPER2, ST_CF_GUARANTEE_SIZE);
	TEST_INSTANCE(ST_PAPER2, ST_FX);
	TEST_INSTANCE(ST_PAPER2, ST_JP_GUARANTEE_DISTANCE);
	TEST_INSTANCE(ST_PAPER2, ST_JP_GUARANTEE_SIZE);
#endif

#if defined(LIB_RATSS_WITH_FPLLL)
	TEST_INSTANCE(ST_PLANE, ST_FPLLL_GUARANTEE_DISTANCE);
	TEST_INSTANCE(ST_SPHERE, ST_FPLLL_GUARANTEE_DISTANCE);
	TEST_INSTANCE(ST_PLANE, ST_FPLLL_GUARANTEE_SIZE);
	TEST_INSTANCE(ST_SPHERE, ST_FPLLL_GUARANTEE_SIZE);
#endif
	
#if defined(LIB_RATSS_WITH_CGAL) and defined(LIB_RATSS_WITH_FPLLL)
	TEST_INSTANCE(ST_PAPER, ST_FPLLL_GUARANTEE_DISTANCE);
	TEST_INSTANCE(ST_PAPER, ST_FPLLL_GUARANTEE_SIZE);
#endif
	
#if defined(LIB_RATSS_WITH_CORE_TWO) and defined(LIB_RATSS_WITH_FPLLL)
	TEST_INSTANCE(ST_PAPER2, ST_FPLLL_GUARANTEE_DISTANCE);
	TEST_INSTANCE(ST_PAPER2, ST_FPLLL_GUARANTEE_SIZE);
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
