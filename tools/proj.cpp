#include <libratss/ProjectSN.h>
#include "../common/stats.h"

typedef enum {
	ST_NONE=0x0,
	ST_SPHERE=0x1,
	ST_PLANE=0x2,
	ST_CF=0x4,
	ST_FT=0x8
} SnapType;

struct Snapper {
	ratss::ProjectSN proj;
	std::vector<mpq_class> snap2Sphere(const std::vector<mpfr::mpreal> & coords_sphere, int st) {
		std::vector<mpq_class> coords_sphere_pq(coords_sphere.size());
		proj.snap(coords_sphere.begin(), coords_sphere.end(), coords_sphere_pq.begin(), st);
		return coords_sphere_pq;
	}
};

void help() {
	std::cout << "prg OPTIONS \n coordinate0 coordinate1 [coordinate2] ...\n"
		"Options:\n"
		"\t-p num\tset the precision of the input in bits\n"
		"\t-r (fp|cf)\tset the type of float->rational conversion. fp=fixpoint, cf=continous fraction\n"
		"\t-s (s|sphere|p|plane)\tset where the float->rational conversion should take place\n"
		"\t-b\talso print bitsize statistics"
		"\t-n\tnormalize input to length 1"
	<< std::endl;
}

int main(int argc, char ** argv) {
	Snapper snapper;
	
	std::vector<mpfr::mpreal> coords_sphere;
	std::vector<mpq_class> coords_sphere_pq;
	
	int precision = -1;
	int st = ST_NONE;
	bool normalize = false;
	bool stats = false;
	
	for(int i(1); i < argc; ++i) {
		std::string token(argv[i]);
		if (token == "-p") {
			if (i+1 < argc) {
				precision = ::atoi(argv[i+1]);
				++i;
			}
			else {
				help();
				return -1;
			}
		}
		else if (token == "-r") {
			if (i+1 < argc) {
				std::string stStr(argv[i+1]);
				if (stStr == "cf") {
					st &= ~ST_FT;
					st |= ST_CF;
				}
				else if (stStr == "ft") {
					st |= ST_FT;
					st &= ~ST_CF;
				}
				++i;
			}
			else {
				help();
				return -1;
			}
		}
		else if (token == "-s") {
			if (i+1 < argc) {
				std::string stStr(argv[i+1]);
				if (stStr == "p" || stStr == "plane") {
					st &= ~ST_SPHERE;
					st |= ST_PLANE;
				}
				else if (stStr == "s" || stStr == "sphere") {
					st |= ST_SPHERE;
					st &= ~ST_PLANE;
				}
				++i;
			}
			else {
				help();
				return -1;
			}
		}
		else if (token == "-n") {
			normalize = true;
		}
		else if (token == "-b") {
			stats = true;
		}
		else {
			try {
				coords_sphere.emplace_back(token);
			}
			catch (std::exception & e) {
				std::cerr << "Error while trying to parse coordinate: " << e.what() << std::endl;
				help();
				return -1;
			}
		}
	}
	if (precision < 0) {
		precision = 32;
	}
	
	if (! (st & (ST_PLANE|ST_SPHERE))) {
		st |= ST_PLANE;
	}
	
	if (! (st & (ST_CF|ST_FT))) {
		st |= ST_FT;
	}
	
	//set the precision of our input variables
	for(mpfr::mpreal & v : coords_sphere) {
		v.setPrecision(precision, MPFR_RNDZ);
	}
	
	if (normalize) {
		snapper.proj.calc().normalize(coords_sphere.begin(), coords_sphere.end(), coords_sphere.begin());
	}
	
	coords_sphere_pq = snapper.snap2Sphere(coords_sphere, st);
	
	ratss::BitCount bc;
	for(const auto & x : coords_sphere_pq) {
		std::cout << x << '\n';
		bc.update(x);
	}
	if (stats) {
		std::cout << bc << std::endl;
	}
	return 0;
}