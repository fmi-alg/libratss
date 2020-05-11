#include <libratss/ProjectSN.h>
#include <libratss/util/BasicCmdLineOptions.h>
#include <libratss/util/InputOutputPoints.h>
#include <libratss/util/InputOutput.h>
#include <libratss/SimApxLLL.h>
#include <libratss/debug.h>

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
	bool printApxQuality{false};
	mpz_class N{0};
public:
	Config() = default;
	using BasicCmdLineOptions::parse;
	virtual bool parse(const std::string & token, int & i, int argc, char ** argv) {
		if (token == "-c") {
			check = true;
		}
		else if (token == "--stats") {
			stats = true;
		}
		else if (token == "--epsilon" && i+1 < argc) {
			epsilon = mpq_class(argv[i+1]);
			if (epsilon <= 0) {
				throw ParseError("An epsilon <= 0 is not allowed");
			}
			++i;
		}
		else if (token == "--with-apx-quality") {
			printApxQuality = true;
		}
		else if (token == "-N" && i+1 < argc) {
			N = mpz_class(std::string(argv[i+1]));
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
			"\t--stats\tprint statistics\n"
			"\t-c\tcheck approximated points\n"
			"\t--with-apx-quality\n"
			"\t--epsilon\tapproximation quality given as rational overrides significands option"
			"\t-N\tMaximum denominator size";
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

struct DenomStats {
	std::size_t errorBetterThanFx{0};
};

std::ostream & operator<<(std::ostream & out, DenomStats const & dstats) {
	return out << "Points better than fx: " << dstats.errorBetterThanFx;
}

int main(int argc, char ** argv) {
	ratss::init_interactive_debuging();
	
	Config cfg;
	ProjectSN proj;
	ratss::BitCount bc;
	DenomStats dstats;

	int ret = cfg.parse(argc, argv); 
	
	if (ret <= 0) {
		cfg.help(std::cerr);
		return ret;
	}
	
	InputOutput io;
	io.setInput(cfg.inFileName);
	io.setOutput(cfg.outFileName);
	

	if (cfg.snapType & ratss::ST_FPLLL_FIXED_N && cfg.N < 2) {
		if (cfg.epsilon > 0) {
			cfg.N = mpz_class(1/cfg.epsilon);
			if (cfg.verbose) {
				io.info() << "Setting N to 1/epsilon=" << cfg.N << std::endl;
			}
		}
		else {
			cfg.N = mpz_class(1) << cfg.significands;
			if (cfg.verbose) {
				io.info() << "Setting N to 2^significands=" << cfg.N << std::endl;
			}
		}
	}
	
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
		
		if (cfg.snapType & ST_FPLLL_MASK) {
			SimApxLLL<RationalPoint::const_iterator> sapx(ip.coords.begin(), ip.coords.end());
			if (cfg.epsilon > 0) {
				sapx.setEps(cfg.epsilon);
			}
			else {
				sapx.setSignificands(cfg.significands);
			}
			if (cfg.N > 1) {
				sapx.setN(cfg.N);
				sapx.run(ST_FPLLL_FIXED_N);
			}
			else {
				sapx.run(SnapType(cfg.snapType & ST_SNAP_TYPES_MASK));
			}
			auto oit = op.coords.begin();
			for(auto it(sapx.numerators_begin()); it != sapx.numerators_end(); ++it, ++oit) {
				*oit = mpq_class(*it, sapx.denominator());
				oit->canonicalize();
			}
			if (cfg.stats && (cfg.snapType & ST_FPLLL_FIXED_N)) {
				mpq_class maxQ = cfg.N * sqrt(mpz_class(1) << ip.coords.size());
				auto mn = ip.c.maxNorm(ip.coords.begin(), ip.coords.end(), op.coords.begin());
				if (mn <= 1/maxQ) {
					++dstats.errorBetterThanFx;
				}
			}
			if (cfg.check && cfg.N > 1) {
				SimApxBruteForce<mpq_class, 0> bfapx(ip.coords.begin(), ip.coords.end());
				bfapx.run(cfg.N.get_ui());
				mpz_class err_fac;
				{
					err_fac = mpz_class(1) << (op.coords.size()-1);
					err_fac *= 5*op.coords.size();
					err_fac = sqrt(err_fac);
				}
				mpz_class max_d_lll = abs(mpz_class(ip.coords[0]*sapx.denominator()));
				mpz_class max_d_bf = abs(mpz_class(ip.coords[0]*bfapx.denominator()));
				for(std::size_t i(1); i < op.coords.size(); ++i) {
					max_d_lll = max(max_d_lll, mpz_class(abs(ip.coords[i]*sapx.denominator())));
					max_d_bf = max(max_d_bf, mpz_class(abs(ip.coords[i]*bfapx.denominator())));
				}
				if (!(max_d_lll <= max_d_bf*err_fac)) {
					std::stringstream ss;
					ss << "Theorem B apx quality violated:";
					ss << "error_fac=" << err_fac;
					ss << " error_bf=" << max_d_bf;
					ss << " error_lll=" << max_d_lll << "=";
					ss << (mpq_class(max_d_lll, mpz_class(1))/mpq_class(max_d_bf), mpz_class(1)).get_d() << "error_bf";
					throw std::runtime_error(ss.str());
				}
			}
		}
		else if (cfg.snapType & ST_BRUTE_FORCE) {
			SimApxBruteForce<mpq_class, 0> sapx(ip.coords.begin(), ip.coords.end());
			if (cfg.N > 0) {
				sapx.run(cfg.N.get_ui());
			}
			else {
				sapx.run(cfg.significands);
			}
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
		if (cfg.check && !(cfg.snapType & ST_FPLLL_MASK)) {
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
		if (cfg.printApxQuality) {
			auto mn = ip.c.maxNorm(ip.coords.begin(), ip.coords.end(),op.coords.begin());
			io.output() << ' ' << mn << ' ' << mn.get_d();
		}
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
		io.info() << dstats << std::endl;
	}
	
	return 0;
}
