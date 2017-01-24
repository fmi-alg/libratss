#include <libratss/ProjectSN.h>
#include <libratss/ProjectS2.h>

#include <random>
#include <chrono>

#ifdef LIB_RATSS_WITH_CGAL
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/point_generators_3.h>
#endif

#include <libratss/util/InputOutputPoints.h>

using namespace LIB_RATSS_NAMESPACE;

typedef enum {GT_NPLANE, GT_NSPHERE, GT_CGAL, GT_GEO, GT_GEOGRID } GeneratorType;

struct PointGenerator {
	ProjectS2 proj;
	GeoCalc gc;

	virtual ~PointGenerator() {}
	virtual RationalPoint generate(int dimension, bool snap) = 0;
	virtual bool supports(int dimension) const = 0;
	template<typename T_FLOAT_ITERATOR>
	RationalPoint toRationalPoint(T_FLOAT_ITERATOR begin, const T_FLOAT_ITERATOR & end, int snapType) {
		using std::distance;
		int dim = distance(begin, end);
		RationalPoint rp(dim);
		if (snapType == ProjectSN::ST_NONE) {
			gc.toRational(begin, end, rp.coords.begin(), ProjectSN::ST_FL, -1);
		}
		else {
			proj.snap(begin, end, rp.coords.begin(), snapType);
		}
	}
};

#ifdef LIB_RATSS_WITH_CGAL

struct CGALPointGenerator: PointGenerator {
	using K = CGAL::Exact_predicates_inexact_constructions_kernel;
	using Rand = CGAL::Random_points_on_sphere_3<K::Point_3>;
	
	Rand rnd;
	
	CGALPointGenerator() : rnd(1.0) {}
	virtual ~CGALPointGenerator() {}
	
	virtual RationalPoint generate(int dimension, bool snap) override {
		K::Point_3 p = *rnd;
		++rnd;
		std::array<mpfr::mpreal, 3> vec = {p.x(), p.y(), p.z()};
		if (snap) {
			return toRationalPoint(vec.begin(), vec.end(), ProjectSN::ST_CF | ProjectSN::ST_SPHERE | ProjectSN::ST_NORMALIZE);
		}
		else {
			return toRationalPoint(vec.begin(), vec.end(), ProjectSN::ST_NONE);
		}
	}
	
	virtual bool supports(int dimension) const override {
		return (dimension == 3);
	}
};
#else
	struct CGALPointGenerator: PointGenerator {
		virtual ~CGALPointGenerator() {}
		virtual RationalPoint generate(int, bool) override {
			return RationalPoint();
		}
		virtual bool supports(int) const override {
			return false;
		}
	};

#endif

struct GeoPointGenerator: PointGenerator {
	std::uniform_real_distribution<double> urdLat;
	std::uniform_real_distribution<double> urdLon;
	std::default_random_engine gen;
	
	GeoPointGenerator() :
		urdLat(-90, 90),
		urdLon(-180, 180),
		gen(std::chrono::system_clock::now().time_since_epoch().count())
	{}
	virtual ~GeoPointGenerator() {}
	
	virtual RationalPoint generate(int dimension, bool snap) override {
		double lat = urdLat(gen);
		double lon = urdLon(gen);
		
		if (snap) {
			RationalPoint ret(3);
			proj.projectFromGeo(lat, lon, ret.coords[0], ret.coords[1], ret.coords[2]);
			return ret;
		}
		else {
			std::array<mpfr::mpreal, 3> c;
			gc.cartesian(lat, lon, c[0], c[1], c[2]);
			return toRationalPoint(c.begin(), c.end(), ProjectSN::ST_NONE);
		}
	}
	
	virtual bool supports(int dimension) const override {
		return (dimension == 3);
	}
};

struct GeoGridGenerator: PointGenerator {
	ProjectS2 proj;
	
	virtual ~GeoGridGenerator() {}
	
	virtual RationalPoint generate(int, bool) override {
		return RationalPoint();
	}
	
	virtual bool supports(int dimension) const override {
		return (dimension == 3);
	}
        
	std::vector<RationalPoint> generateAll( uint32_t nofSlices ){
		// nofSlices >= 1== number of slices on an eighth sphere
		std::vector<RationalPoint> result(0);
		if( nofSlices < 1)
			return result;
		
		RationalPoint northPole(3);
		northPole.coords = { 0, 0, 1 };
		result.push_back( northPole );     // north pole
		
		// lat in [-90,+90]
		// lon in [0,360)
		
		const double angleInc = 90.0 / nofSlices;
		
		for( int sLat=1; sLat <2*nofSlices; ++sLat ){
			double lat = +90.0 - angleInc * sLat;
			
			for( int sLon=0; sLon < 4*nofSlices; ++sLon ){
				RationalPoint ret(3);
				double lon = angleInc * sLon;
				proj.projectFromGeo( mpfr::mpreal(lat), mpfr::mpreal(lon), ret.coords[0], ret.coords[1], ret.coords[2]);
				result.push_back( ret );
			}
		}
		
		RationalPoint southPole(3);
		southPole.coords = { 0, 0, -1 };
		result.push_back( southPole );     // south pole
		
		return result;
	}
};

struct NPlanePointGenerator: PointGenerator {
	std::uniform_real_distribution<double> circleRnd;
	std::uniform_int_distribution<uint32_t> boolRnd;
	std::default_random_engine gen;
	ProjectSN proj;
	
	NPlanePointGenerator() : circleRnd(-1.0, 1.0), boolRnd(0, 1) {}
	virtual ~NPlanePointGenerator() {}
	
	void gen_plane_point(std::size_t count, FloatPoint & ip) {
		ip.coords.resize(count);
		for(mpfr::mpreal & v : ip.coords) {
			v = circleRnd(gen);
		}
	}
	
	virtual RationalPoint generate(int dimension, bool snap) override {
		FloatPoint ip;
		RationalPoint ret;
		ret.resize(dimension); 
		while (true) {
			gen_plane_point(dimension-1, ip);
			if (ip.sqLen() <= 1.0) {
				break;
			}
		}
		ip.coords.emplace_back(0);
		bool positiveSide = boolRnd(gen);
		if (snap) {
			std::vector<mpq_class> snapVec(ip.coords.size());
			proj.calc().toRational(ip.coords.begin(), ip.coords.end(), snapVec.begin(), Calc::ST_CF);
			assert(snapVec.size() == dimension);
			proj.plane2Sphere(snapVec.begin(), snapVec.end(), (PositionOnSphere)(positiveSide ? dimension : -dimension), ret.coords.begin());
			return ret;
		}
		else {
			std::vector<mpfr::mpreal> onSphere(dimension);
			proj.plane2Sphere(ip.coords.begin(), ip.coords.end(), (PositionOnSphere)(positiveSide ? dimension : -dimension), onSphere.begin());
			return toRationalPoint(onSphere.begin(), onSphere.end(), ProjectSN::ST_NONE);
		}
	}
	
	virtual bool supports(int dimension) const override {
		return (dimension > 1);
	}
};

struct NSpherePointGenerator: PointGenerator {
	std::normal_distribution<double> circleRnd;
	std::default_random_engine gen;
	ProjectSN proj;
	
	NSpherePointGenerator() : circleRnd(0.0, 1.0) {}
	virtual ~NSpherePointGenerator() {}
	
	virtual RationalPoint generate(int dimension, bool snap) override {
		std::vector<mpfr::mpreal> vec; 
		for(int i(0); i < dimension; ++i) {
			vec.emplace_back( circleRnd(gen) );
		}
		if (snap) {
			return toRationalPoint(vec.begin(), vec.end(), ProjectSN::ST_FX | ProjectSN::ST_SPHERE | ProjectSN::ST_NORMALIZE);
		}
		else {
			return toRationalPoint(vec.begin(), vec.end(), ProjectSN::ST_NONE);
		}
	}
	
	virtual bool supports(int dimension) const override {
		return (dimension > 1);
	}
};

void help(std::ostream & out) {
	out << "prg OPTIONS\n"
		"Options:\n"
		"-g generator\tgenerator = (nplane|nsphere|cgal|geo|geogrid)\n"
		"-f format\tformat = (rational|split|float|float128|geo|spherical)\n"
		"-d dimensions\n"
		"-n number\tnumber of points to create\n"
		"--no-snap\tdon't snap points to the sphere"
		<< std::endl;
}

struct Config {
	GeneratorType gt;
	RationalPoint::Format ft;
	int dimension;
	uint32_t count;
	bool snap;
	
	Config() : gt(GT_NPLANE), ft(RationalPoint::FM_RATIONAL), dimension(3), count(0), snap(true) {}
	
	int parse(int argc, char ** argv) {
		for(int i(1); i < argc; ++i) {
			std::string token(argv[i]);
			if (token == "-g") {
				if (i+1 < argc) {
					std::string gtStr(argv[i+1]);
					if (gtStr == "nplane") {
						gt = GT_NPLANE;
					}
					else if (gtStr == "nsphere") {
						gt = GT_NSPHERE;
					}
					else if (gtStr == "cgal") {
						gt = GT_CGAL;
					}
					else if (gtStr == "geo") {
						gt = GT_GEO;
					}
					else if (gtStr == "geogrid" || gtStr == "geoGrid"){
						gt = GT_GEOGRID;
					}
					else {
						std::cerr << "Unsupported generator: " << gtStr << std::endl;
						return -1;
					}
				}
				else {
					help(std::cerr);
					return -1;
				}
				++i;
			}
			else if (token == "-f") {
				if (i+1 < argc) {
					std::string ftStr(argv[i+1]);
					if (ftStr == "rational") {
						ft = RationalPoint::FM_RATIONAL;
					}
					else if (ftStr == "split") {
						ft = RationalPoint::FM_SPLIT_RATIONAL;
					}
					else if (ftStr == "float") {
						ft = RationalPoint::FM_FLOAT;
					}
					else if (ftStr == "float128") {
						ft = RationalPoint::FM_FLOAT128;
					}
					else if (ftStr == "geo") {
						ft = RationalPoint::FM_GEO;
					}
					else if (ftStr == "spherical") {
						ft = RationalPoint::FM_SPHERICAL;
					}
					else {
						std::cerr << "Unsupported output format: " << ftStr << std::endl;
						return -1;
					}
				}
				else {
					help(std::cerr);
					return -1;
				}
				++i;
			}
			else if (token == "-d") {
				if (i+1 < argc) {
					dimension = ::atoi(argv[i+1]);
					++i;
				}
				else {
					help(std::cerr);
					return -1;
				}
			}
			else if (token == "-n") {
				if (i+1 < argc) {
					count = ::atoi(argv[i+1]);
					++i;
				}
				else {
					help(std::cerr);
					return -1;
				}
			}
			else if (token == "-h" || token == "--help") {
				help(std::cout);
				return 0;
			}
			else if (token == "--no-snap") {
				snap = false;
			}
		}
		
		if (dimension != 3 && (ft == RationalPoint::FM_SPHERICAL || ft == RationalPoint::FM_GEO)) {
			std::cerr << "Forcing dimension to 3" << std::endl;
			dimension = 3;
		}
		
		return 1;
	}
};

int main(int argc, char ** argv) {
	Config cfg;
	PointGenerator * pg = 0;
	
	int ret = cfg.parse(argc, argv);
	if (ret <= 0) {
		return ret;
	}
	
	if (cfg.gt == GT_NPLANE) {
		pg = new NPlanePointGenerator();
	}
	else if (cfg.gt == GT_NSPHERE) {
		pg = new NSpherePointGenerator();
	}
	else if (cfg.gt == GT_CGAL) {
		pg = new CGALPointGenerator();
	}
	else if (cfg.gt == GT_GEO) {
		pg = new GeoPointGenerator();
	}
	else if (cfg.gt == GT_GEOGRID) {
		GeoGridGenerator myPg;
		if (!myPg.supports(cfg.dimension)) {
			std::cerr << "Selected generator does not support the selected dimension" << std::endl;
			return -1;
		}
		std::vector<RationalPoint> gridPoints = myPg.generateAll( cfg.count );
		for( RationalPoint & p : gridPoints ){
			p.print(std::cout, cfg.ft);
			std::cout << '\n';
		}
		return 0;
	}

	if (!pg->supports(cfg.dimension)) {
		std::cerr << "Selected generator does not support the selected dimension" << std::endl;
		delete pg;
		return -1;
	}
	
	for(uint32_t i(0); i < cfg.count; ++i) {
		RationalPoint p = pg->generate(cfg.dimension, cfg.snap);
		p.print(std::cout, cfg.ft);
		std::cout << '\n';
	}
	delete pg;
	return 0;
}