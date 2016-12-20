#include <libratss/ProjectS2.h>
#include "../common/generators.h"
#include "../common/stats.h"

void help() {
	std::cout << "prg [-i <file with one point per line> [-r <number of random points>] [-p <precisio>] [-e <manual eps>]" << std::endl;
}

using namespace LIB_RATSS_NAMESPACE;

int main(int argc, char ** argv) {
	std::size_t num_rand_points = 0;
	std::string inFile;
	int initialPrecision = 32;
	int manualEps = -1;
	mpq_class manEpsVal(1);
	
	for(int i(0); i < argc; ++i) {
		std::string token(argv[i]);
		if (token == "-r" && i+1 < argc) {
			num_rand_points = ::atoi(argv[i+1]);
			++i;
		}
		else if (token == "-i" && i+1 < argc) {
			inFile = std::string(argv[i+1]);
			++i;
		}
		else if (token == "-p" && i+1 < argc) {
			initialPrecision = ::atoi(argv[i+1]);
			++i;
		}
		else if (token == "-e" && i+1 < argc) {
			manualEps = ::atoi(argv[i+1]);
			if (manualEps <= 0) {
				std::cerr << "Epsilon needs to be larger than 0" << std::endl;
				return -1;
			}
			manEpsVal >>= manualEps;
			++i;
		}
		else if (token == "-h" || token == "--help") {
			help();
			return 0;
		}
	}
	if (num_rand_points == 0 && inFile.empty()) {
		help();
		return -1;
	}
	
	std::vector<SphericalCoord> points;
	
	if (inFile.size()) {
		struct MyIt {
			std::vector<SphericalCoord> & points;
			MyIt(std::vector<SphericalCoord> & points) : points(points) {}
			MyIt(const MyIt & other) : points(other.points) {}
			MyIt & operator*() { return *this; }
			MyIt & operator++() { return *this; }
			MyIt & operator=(const GeoCoord & c) {
				points.emplace_back(c);
			}
		};
		readPoints(inFile, MyIt(points));
		std::cout << "Read " << points.size() << " points from " << inFile << std::endl;
	}
	
	if (num_rand_points) {
		ratss::getRandomPolarPoints(num_rand_points, std::back_inserter(points));
	}
	
	BitCount snapFrac, snapFl, snapFx, snapManEps;
	BitCount snapFracProj, snapFlProj, snapFxProj, snapManEpsProj;
	
	std::cout << "Points: " << points.size() << std::endl;
	std::cout << "Precision: " << initialPrecision << std::endl;
	if (manualEps > 0) {
		std::cout << "Manual eps: " << manualEps << std::endl;
	}
	
	ratss::ProjectS2 p;
	ratss::GeoCalc calc;
	mpfr::mpreal xd, yd, zd, xpd, ypd, zpd;
	mpq_class xs, ys, zs, xps, yps, zps;
	for(std::size_t i(0), s(points.size()); i < s; ++i) {
		const SphericalCoord & c = points[i];
		mpfr::mpreal theta(c.theta, initialPrecision+6);
		mpfr::mpreal phi(c.phi, initialPrecision+6);
		calc.cartesianFromSpherical(theta, phi, xd, yd, zd);
		
		xd.setPrecision(initialPrecision, MPFR_RNDZ);
		yd.setPrecision(initialPrecision, MPFR_RNDZ);
		zd.setPrecision(initialPrecision, MPFR_RNDZ);
		
		//we want the bitsize for the snapping in the plane
		auto pos = p.sphere2Plane(xd, yd, zd, xpd, ypd, zpd);
		
		assert(xpd*xpd + ypd*ypd + zpd*zpd <= 1);
		
		//snap with cast
		xps = calc.snap(xpd, Calc::ST_FL);
		yps = calc.snap(ypd, Calc::ST_FL);
		zps = calc.snap(zpd, Calc::ST_FL);
		p.plane2Sphere(xps, yps, zps, pos, xs, ys, zs);
		assert(xs*xs + ys*ys + zs*zs == mpq_class(1));
		assert(xs.get_den() == ys.get_den() && xs.get_den() == zs.get_den());
		snapFl.update(xps);
		snapFl.update(yps);
		snapFl.update(zps);
		snapFlProj.update(xs);
		snapFlProj.update(ys);
		snapFlProj.update(zs);
		
		//snap with fix point
		xps = calc.snap(xpd, Calc::ST_FX);
		yps = calc.snap(ypd, Calc::ST_FX);
		zps = calc.snap(zpd, Calc::ST_FX);
		p.plane2Sphere(xps, yps, zps, pos, xs, ys, zs);
		assert(xs*xs + ys*ys + zs*zs == mpq_class(1));
		assert(xs.get_den() == ys.get_den() && xs.get_den() == zs.get_den());
		snapFx.update(xps);
		snapFx.update(yps);
		snapFx.update(zps);
		snapFxProj.update(xs);
		snapFxProj.update(ys);
		snapFxProj.update(zs);
		
		//snap with continous fraction
		xps = calc.snap(xpd, Calc::ST_CF);
		yps = calc.snap(ypd, Calc::ST_CF);
		zps = calc.snap(zpd, Calc::ST_CF);
		p.plane2Sphere(xps, yps, zps, pos, xs, ys, zs);
		assert(xs*xs + ys*ys + zs*zs == mpq_class(1));
		assert(xs.get_den() == ys.get_den() && xs.get_den() == zs.get_den());
		snapFrac.update(xps);
		snapFrac.update(yps);
		snapFrac.update(zps);
		snapFracProj.update(xs);
		snapFracProj.update(ys);
		snapFracProj.update(zs);
		
		//snap with continous fraction with manual eps
		if (manualEps > 0) {
			xps = Conversion<mpfr::mpreal>::toMpq(xpd);
			yps = Conversion<mpfr::mpreal>::toMpq(ypd);
			zps = Conversion<mpfr::mpreal>::toMpq(zpd);
			xps = calc.within(xps-manEpsVal, xps+manEpsVal);
			yps = calc.within(yps-manEpsVal, yps+manEpsVal);
			zps = calc.within(zps-manEpsVal, zps+manEpsVal);
			p.plane2Sphere(xps, yps, zps, pos, xs, ys, zs);
			assert(xs*xs + ys*ys + zs*zs == mpq_class(1));
			assert(xs.get_den() == ys.get_den() && xs.get_den() == zs.get_den());
			snapManEps.update(xps);
			snapManEps.update(yps);
			snapManEps.update(zps);
			snapManEpsProj.update(xs);
			snapManEpsProj.update(ys);
			snapManEpsProj.update(zs);
		}
		if (i % 1000 == 0) {
			std::cout << '\xd' << i/1000 << "k" << std::flush;
		}
	}
	std::cout << std::endl;
	
	std::cout << "Bit sizes with snapping with float:\n" << snapFl << '\n';
	std::cout << "Bit sizes with snapping with fix point:\n" << snapFx << '\n';
	std::cout << "Bit sizes with snapping by continous fraction:\n" << snapFrac << '\n';
	if (manualEps > 0) {
		std::cout << "Bit sizes with snapping by continous fraction with given eps:\n" << snapManEps << '\n';
	}
	std::cout << "Bit sizes with projection and snapping with float:\n" << snapFlProj << '\n';
	std::cout << "Bit sizes with projection and snapping with fixpoint:\n" << snapFxProj << '\n';
	std::cout << "Bit sizes with projection and snapping by continous fraction::\n" << snapFracProj << '\n';
	if (manualEps > 0) {
		std::cout << "Bit sizes with projection and snapping by continous fraction with given eps:\n" << snapManEpsProj << '\n';
	}
	std::cout << std::flush;
	return 0;
}