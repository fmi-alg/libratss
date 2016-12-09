#include <libratss/ProjectSN.h>

typedef enum {
	ST_NONE=0x0,
	ST_SPHERE=0x1,
	ST_PLANE=0x2,
	ST_CF=0x4,
	ST_FT=0x8
} SnapType;

struct Snapper {
	ratss::ProjectSN proj;
	
	
	void snap(const std::vector<mpfr::mpreal> & floatCoords, std::vector<mpq_class> & ratCoords, int st) {
		ratCoords.resize(floatCoords.size());
		if (st & ST_CF) {
			//snap plane coordinates to rationals
			std::transform(
				floatCoords.begin(),
				floatCoords.end(),
				ratCoords.begin(),
				[this](const mpfr::mpreal & v) {
					return proj.calc().snap(v);
				}
			);
		}
		else {
			//snap plane coordinates to rationals
			std::transform(
				floatCoords.begin(),
				floatCoords.end(),
				ratCoords.begin(),
				[this](const mpfr::mpreal & v) {
					return mpq_class( ratss::Conversion<mpfr::mpreal>::toMpq( proj.calc().toFixpoint(v) ) );
				}
			);
		}
	}

	ratss::PositionOnSphere sphere2Plane(const std::vector<mpfr::mpreal> & coords_sphere, std::vector<mpq_class> & coords_plane_pq, int st) {
		coords_plane_pq.resize(coords_sphere.size());
		
		ratss::PositionOnSphere pos;
		if (st & ST_SPHERE) {
			std::vector<mpq_class> coords_sphere_pq(coords_sphere.size());
			snap(coords_sphere, coords_sphere_pq, st);
			pos = proj.sphere2Plane(coords_sphere_pq.begin(), coords_sphere_pq.end(), coords_plane_pq.begin());
		}
		else {
			std::vector<mpfr::mpreal> coords_plane(coords_sphere.size());
			pos = proj.sphere2Plane(coords_sphere.begin(), coords_sphere.end(), coords_plane.begin());
			snap(coords_plane, coords_plane_pq, st);
		}
		return pos;
	}
	
	void plane2Sphere(const std::vector<mpq_class> & coords_plane_pq, ratss::PositionOnSphere pos,  std::vector<mpq_class> & coords_sphere_pq) {
		proj.plane2Sphere(coords_plane_pq.begin(), coords_plane_pq.end(), pos, coords_sphere_pq.end());
	}
	
	std::vector<mpq_class> snap2Sphere(const std::vector<mpfr::mpreal> & coords_sphere, int st) {
		std::vector<mpq_class> coords_plane_pq;
		std::vector<mpq_class> coords_sphere_pq;
		auto pos = sphere2Plane(coords_sphere, coords_plane_pq, st);
		plane2Sphere(coords_plane_pq, pos, coords_sphere_pq);
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
	<< std::endl;
}

int main(int argc, char ** argv) {
	Snapper snapper;
	
	std::vector<mpfr::mpreal> coords_sphere;
	std::vector<mpq_class> coords_sphere_pq;
	
	int precision = -1;
	int st = ST_NONE;
	
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
	
	coords_sphere_pq = snapper.snap2Sphere(coords_sphere, st);
	
	for(const auto & x : coords_sphere_pq) {
		std::cout << x << '\n';
	}
	return 0;
}