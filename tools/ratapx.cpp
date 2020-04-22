#include <libratss/ProjectSN.h>
#include <libratss/util/BasicCmdLineOptions.h>
#include <libratss/util/InputOutputPoints.h>
#include <libratss/util/InputOutput.h>
#include <libratss/SimApxLLL.h>

#include "../common/stats.h"
#include <fstream>
#include "types.h"

using namespace LIB_RATSS_NAMESPACE;
using namespace LIB_RATSS_NAMESPACE::tools;

class Config: public BasicCmdLineOptions {
public:
	bool stats{false};
	bool check{false};
	mpq_class epsilon{-1};
public:
	Config() = default;
	using BasicCmdLineOptions::parse;
	virtual bool parse(const std::string & token, int & i, int argc, char ** argv) {
		if (token == "-c") {
			check = true;
		}
		else if (token == "-b") {
			stats = true;
		}
		else if (token == "--epsilon" && i+1 < argc) {
			epsilon = mpq_class(argv[i+1]);
			if (epsilon <= 0) {
				throw ParseError("An epsilon <= 0 is not allowed");
			}
			++i;
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
			"\t-c\tcheck approximated points\n"
			"\t--epsilon\tapproximation quality given as rational overrides significands option";
		BasicCmdLineOptions::options_help(out);
		out << std::endl;
	}
	void print(std::ostream & out) const {
		out << "Check: " << (check ? "yes" : "no") << '\n';
		out << "Stats: " << (check ? "yes" : "no") << '\n';
		if (epsilon > 0) {
			out << "Epsilon: " << epsilon << '\n';
		}
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
	
	RationalPoint ip;
	RationalPoint op;
	
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
		
		ip.assign(io.input(), cfg.inFormat, cfg.precision);
		op.clear();
		op.resize(ip.coords.size());
		
		if (cfg.snapType & (ST_FPLLL | ST_FPLLL_GREEDY) && cfg.epsilon > 0) {
			SimApxLLL<RationalPoint::const_iterator> sapx(ip.coords.begin(), ip.coords.end(), cfg.epsilon);
			sapx.run(SnapType(cfg.snapType & ST_SNAP_POSITION_MASK));
			auto oit = op.coords.begin();
			for(auto it(sapx.numerators_begin()); it != sapx.numerators_end(); ++it, ++oit) {
				*oit = mpq_class(*it, sapx.denominator());
				oit->canonicalize();
			}
		}
		else if (cfg.snapType & ST_BRUTE_FORCE) {
			SimApxBruteForce<mpq_class, 0> sapx(ip.coords.begin(), ip.coords.end());
			sapx.run(cfg.significands);
			auto oit = op.coords.begin();
			for(auto it(sapx.numerators_begin()); it != sapx.numerators_end(); ++it, ++oit) {
				*oit = *it/sapx.denominator();
			}
		}
		else {
			FloatPoint fip;
			fip.assign(ip.coords.begin(), ip.coords.end(), cfg.precision);
			proj.calc().toRational(fip.coords.begin(), fip.coords.end(), op.coords.begin(), cfg.snapType & ST_SNAP_TYPES_MASK, cfg.significands);
		}

		if (cfg.stats) {
			bc.update(op.coords.begin(), op.coords.end());
		}
		if (cfg.check) {
			auto mn = proj.calc().maxNorm(ip.coords.begin(), ip.coords.end(), op.coords.begin());
			mpq_class eps(mpz_class(1), mpz_class(1) << cfg.significands);
			if (mn > eps) {
				io.info() << "Invalid approximation for point ";
				ip.print(io.info(), cfg.outFormat);
				io.info() << std::endl;
				
				io.info() << "Incorrect approximation: ";
				op.print(io.info(), cfg.outFormat);
				io.info() << std::endl;
				
				return -1;
			}
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
