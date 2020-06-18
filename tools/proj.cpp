#include <libratss/ProjectSN.h>
#include <libratss/util/BasicCmdLineOptions.h>
#include <libratss/util/InputOutputPoints.h>
#include <libratss/util/InputOutput.h>

#include "../common/stats.h"
#include <fstream>
#include "types.h"

using namespace LIB_RATSS_NAMESPACE;
using namespace LIB_RATSS_NAMESPACE::tools;

class Config: public BasicCmdLineOptions {
public:
	bool stats;
	bool check;
	bool planeCoords;
public:
	Config() :
	stats(false),
	check(false),
	planeCoords(false)
	{}
	using BasicCmdLineOptions::parse;
	bool parse(const std::string & token, int &, int , char **) override {
		if (token == "-c") {
			check = true;
		}
		else if (token == "-b") {
			stats = true;
		}
		else if (token == "--print-plane-coords")  {
			planeCoords = true;
		}
		else {
			return false;
		}
		return true;
	}
	
	void help(std::ostream & out) const {
		out << "prg OPTIONS\n"
			"Options:\n"
			"\t-b\talso print bitsize statistics\n"
			"\t--print-plane-coords\tprint coordinates in the plane\n"
			"\t-c\tcheck projected points\n";
		BasicCmdLineOptions::options_help(out);
		out << std::endl;
	}
	void print(std::ostream & out) const {
		out << "Check: " << (check ? "yes" : "no") << '\n';
		BasicCmdLineOptions::options_selection(out);
	}
};

int main(int argc, char ** argv) {
	Config cfg;
	ProjectSN proj;
	ratss::BitCount bc;

	int ret = cfg.parse(argc, argv); 
	
	if (ret <= 0) {
		cfg.help(std::cerr);
		return ret;
	}
	
	InputOutput io;
	io.setInput(cfg.inFileName);
	io.setOutput(cfg.outFileName);
	
	if (cfg.verbose) {
		cfg.print(io.info());
		io.info() << std::endl;
	}
	
	FloatPoint ip;
	RationalPoint op;
	RationalPoint opp;
	
	if (cfg.progress) {
		io.info() << std::endl;
	}
	std::size_t counter = 0;
	while( io.input().good() ) {
		for( ; io.input().good() && io.input().peek() == '\n'; ) {
			io.input().get();
			io.output().put('\n');
		}
		if (!io.input().good()) {
			break;
		}
		bool opFromIp = !cfg.rationalPassThrough;
		if (cfg.rationalPassThrough) {
			op.assign(io.input(), cfg.inFormat, cfg.precision);
			if (!op.valid()) {
				if (!cfg.normalize) {
					std::cerr << "Input point read that is not on sphere but no normalization was requested" << std::endl;
				}
				else {
					ip.assign(op.coords.begin(), op.coords.end(), cfg.precision);
					opFromIp = true;
				}
			}
		}
		else {
			ip.assign(io.input(), cfg.inFormat, cfg.precision);
		}
		
		if (opFromIp) {
			if (cfg.normalize) {
				if (cfg.verbose) {
					io.info() << "Normalizing (" << ip << ") to ";
				}
				ip.normalize();
				if (cfg.verbose) {
					io.info() << '(' << ip << ')' << '\n';
				}
			}
			ip.setPrecision(cfg.precision);
			op.clear();
			op.resize(ip.coords.size());
			proj.snap(ip.coords.begin(), ip.coords.end(), op.coords.begin(), cfg.snapType, cfg.significands);
			if (cfg.planeCoords) {
				opp.clear();
				opp.resize(op.coords.size());
				auto pos = proj.sphere2Plane(op.coords.begin(), op.coords.end(), opp.coords.begin());
				using std::swap;
				swap(opp.coords, op.coords);
			}
		}
		if (cfg.stats) {
			bc.update(op.coords.begin(), op.coords.end());
		}
		if (cfg.check) {
			if (!op.valid()) {
				io.info() << "Invalid projection for point " << ip << std::endl;
			}
			for(std::size_t i(0), s(op.coords.size()); i < s; ++i) {
				auto const & opc = op.coords[i];
				auto const & ipc = ip.coords[i];
				if (cfg.snapType & (ST_GUARANTEE_SIZE|ST_FX)) {
					auto numBits = ::mpz_sizeinbase(opc.get_den_mpz_t(), 2);
					if (numBits > 2*cfg.significands+1) {
						io.info() << "Point " << counter << " exceeds requested denominator size" << std::endl;
					} 
				}
				if (cfg.snapType & (ST_GUARANTEE_DISTANCE|ST_FX|ST_FL)) {
					if (abs(opc - convert<mpq_class>(ipc)) > mpq_class(mpz_class(1), mpz_class(1) << cfg.significands)) {
						io.info() << "Point " << counter << " exceeds requested distance" << std::endl;
					}
				}
			}
			return -1;
		}
		op.print(io.output(), cfg.outFormat);
		if (io.input().peek() != '\n') {
			io.output().put(' ');
		}
		
		++counter;
		if (cfg.progress && counter % 1000 == 0) {
			io.info() << '\xd' << counter/1000 << "k" << std::flush;
		}
	}
	
	if (cfg.stats) {
		io.info() << bc << std::endl;
	}
	
	return 0;
}
