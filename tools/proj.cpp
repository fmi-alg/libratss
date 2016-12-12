#include <libratss/ProjectSN.h>
#include "../common/stats.h"

using namespace LIB_RATSS_NAMESPACE;

struct Snapper {
	ProjectSN proj;
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
		"\t-b\talso print bitsize statistics\n"
		"\t-n\tnormalize input to length 1"
	<< std::endl;
}

struct Config {


};

int main(int argc, char ** argv) {
	Snapper snapper;
	
	std::vector<mpfr::mpreal> coords_sphere;
	std::vector<mpq_class> coords_sphere_pq;
	
	int precision = -1;
	int st = ProjectSN::ST_NONE;
	bool normalize = false;
	bool stats = false;
	bool verbose = false;
	
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
					st &= ~ProjectSN::ST_FT;
					st |= ProjectSN::ST_CF;
				}
				else if (stStr == "ft") {
					st |= ProjectSN::ST_FT;
					st &= ~ProjectSN::ST_CF;
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
					st &= ~ProjectSN::ST_SPHERE;
					st |= ProjectSN::ST_PLANE;
				}
				else if (stStr == "s" || stStr == "sphere") {
					st |= ProjectSN::ST_SPHERE;
					st &= ~ProjectSN::ST_PLANE;
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
		else if (token == "-v" || token == "--verbose") {
			verbose = true;
		}
		else if (token == "-h" || token == "--help") {
			help();
			return 0;
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
	
	if (!coords_sphere.size()) {
		help();
		return -1;
	}
	
	if (precision < 0) {
		precision = 32;
	}
	
	if (! (st & (ProjectSN::ST_PLANE|ProjectSN::ST_SPHERE))) {
		st |= ProjectSN::ST_PLANE;
	}
	
	if (! (st & (ProjectSN::ST_CF|ProjectSN::ST_FT))) {
		st |= ProjectSN::ST_FT;
	}
	
	if (verbose) {
		std::cout << "Precision: " << precision << '\n';
		std::cout << "Float conversion method: " << (st & ratss::ProjectSN::ST_FT ? "fixed point" : "continous fraction") << '\n';
		std::cout << "Float conversion location: " << (st & ratss::ProjectSN::ST_SPHERE ? "sphere" : "plane") << '\n';
		std::cout << "Normalize: " << (normalize ? "yes" : "no") << '\n';
	}
	
	//set the precision of our input variables
	for(mpfr::mpreal & v : coords_sphere) {
		v.setPrecision(precision, MPFR_RNDZ);
	}
	
	if (normalize) {
		snapper.proj.calc().normalize(coords_sphere.begin(), coords_sphere.end(), coords_sphere.begin());
		if (verbose) {
			std::cout << "Normalized:\n";
			for(const auto & x : coords_sphere) {
				std::cout << x << '\n';
			}
		}
	}
	
	coords_sphere_pq = snapper.snap2Sphere(coords_sphere, st);
	
	ratss::BitCount bc;
	for(const auto & x : coords_sphere_pq) {
		std::cout << x << '\n';
		if (stats) {
			bc.update(x);
		}
	}
	if (stats) {
		std::cout << bc << std::endl;
	}
	return 0;
}