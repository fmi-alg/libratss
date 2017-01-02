#include <libratss/ProjectSN.h>
#include <libratss/util/BasicCmdLineOptions.h>
#include <libratss/util/InputOutputPoints.h>

#include "../common/stats.h"
#include <fstream>
#include "types.h"

using namespace LIB_RATSS_NAMESPACE;
using namespace LIB_RATSS_NAMESPACE::tools;

std::ostream & operator<<(std::ostream & out, const InputPoint & ip) {
	ip.print(out);
	return out;
}

class Config: public BasicCmdLineOptions {
public:
	bool stats;
	bool check;
public:
	Config() :
	stats(false),
	check(false)
	{}
	using BasicCmdLineOptions::parse;
	virtual bool parse(const std::string & token, int &, int , char **) {
		if (token == "-c") {
			check = true;
		}
		else if (token == "-b") {
			stats = true;
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
	
	if (cfg.verbose) {
		cfg.print(std::cerr);
	}
	
	std::istream * inFile = 0;
	std::ostream * outFile = 0;
	
	std::ifstream inFileHandle;
	std::ofstream outFileHandle;
	
	if (cfg.inFileName.size()) {
		inFileHandle.open(cfg.inFileName);
		if (!inFileHandle.is_open()) {
			std::cerr << "Could not open input file: " << cfg.inFileName << '\n';
			return -1;
		}
		inFile = &inFileHandle;
	}
	else {
		inFile = &std::cin;
	}
	
	if (cfg.outFileName.size()) {
		outFileHandle.open(cfg.outFileName);
		if (!outFileHandle.is_open()) {
			std::cerr << "Could not open output file: " << cfg.outFileName << '\n';
			return -1;
		}
		outFile = &outFileHandle;
	}
	else {
		outFile = &std::cout;
	}
	
	InputPoint ip;
	OutputPoint op;
	
	if (cfg.progress) {
		std::cout << std::endl;
	}
	std::size_t counter = 0;
	while( inFile->good() ) {
		for( ; inFile->good() && inFile->peek() == '\n'; ) {
			inFile->get();
			outFile->put('\n');
		}
		if (!inFile->good()) {
			break;
		}
		ip.assign(*inFile, cfg.inFormat, cfg.precision);
		if (cfg.normalize) {
			if (cfg.verbose) {
				std::cerr << "Normalizing (" << ip << ") to ";
			}
			ip.normalize();
			if (cfg.verbose) {
				std::cerr << '(' << ip << ')' << '\n';
			}
		}
		ip.setPrecision(cfg.precision);
		op.clear();
		op.resize(ip.coords.size());
		proj.snap(ip.coords.begin(), ip.coords.end(), op.coords.begin(), cfg.snapType, cfg.significands);
		if (cfg.stats) {
			bc.update(op.coords.begin(), op.coords.end());
		}
		if (cfg.check && !op.valid()) {
			std::cerr << "Invalid projection for point " << ip << std::endl;
			return -1;
		}
		op.print(*outFile, cfg.outFormat);
		if (inFile->peek() != '\n') {
			outFile->put(' ');
		}
		
		++counter;
		if (cfg.progress && counter % 1000 == 0) {
			std::cout << '\xd' << counter/1000 << "k" << std::flush;
		}
	}
	
	if (cfg.stats) {
		std::cerr << bc << std::endl;
	}
	
	return 0;
}