#include <libratss/ProjectSN.h>
#include "../common/stats.h"
#include <fstream>
#include "types.h"

using namespace LIB_RATSS_NAMESPACE;
using namespace LIB_RATSS_NAMESPACE::tools;

std::ostream & operator<<(std::ostream & out, const InputPoint & ip) {
	ip.print(out);
	return out;
}

void help() {
	std::cout << "prg OPTIONS\n"
		"Options:\n"
		"\t-v\tverbose\n"
		"\t-p num\tset the precision of the input in bits\n"
		"\t-r (fp|cf)\tset the type of float->rational conversion. fp=fixpoint, cf=continous fraction\n"
		"\t-s (s|sphere|p|plane)\tset where the float->rational conversion should take place\n"
		"\t-b\talso print bitsize statistics\n"
		"\t-n\tnormalize input to length 1\n"
		"\t-f format\tset format of output: [rational, split, float]\n"
		"\t-i\tpath to input\n"
		"\t-o\tpath to output"
	<< std::endl;
}

struct Config {
	std::string inFileName;
	std::string outFileName;
	int precision;
	int snapType;
	bool normalize;
	bool stats;
	bool verbose;
	bool check;
	OutputPoint::Format outFormat;
	
	Config() :
	precision(-1),
	snapType(ProjectSN::ST_NONE),
	normalize(false),
	stats(false),
	verbose(false),
	check(false),
	outFormat(OutputPoint::FM_RATIONAL)
	{}
	int parse(int argc, char ** argv) {
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
						snapType |= ProjectSN::ST_CF;
					}
					else if (stStr == "fx") {
						snapType |= ProjectSN::ST_FX;
					}
					else if (stStr == "fl") {
						snapType |= ProjectSN::ST_FL;
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
						snapType &= ~ProjectSN::ST_SPHERE;
						snapType |= ProjectSN::ST_PLANE;
					}
					else if (stStr == "s" || stStr == "sphere") {
						snapType |= ProjectSN::ST_SPHERE;
						snapType &= ~ProjectSN::ST_PLANE;
					}
					++i;
				}
				else {
					help();
					return -1;
				}
			}
			else if (token == "-i") {
				if (i+1 < argc) {
					inFileName.assign(argv[i+1]);
					++i;
				}
				else {
					help();
					return -1;
				}
			}
			else if (token == "-o") {
				if (i+1 < argc) {
					outFileName.assign(argv[i+1]);
					++i;
				}
				else {
					help();
					return -1;
				}
			}
			else if (token == "-f") {
				if (i+1 < argc) {
					std::string stStr(argv[i+1]);
					if (stStr == "rational" || stStr == "rat" || stStr == "r") {
						outFormat = OutputPoint::FM_RATIONAL;
					}
					else if (stStr == "split" || stStr == "sr" || stStr == "s") {
						outFormat = OutputPoint::FM_RATIONAL;
					}
					else if (stStr == "float" || stStr == "double" || stStr == "d" || stStr == "f") {
						outFormat = OutputPoint::FM_RATIONAL;
					}
					++i;
				}
				else {
					help();
					return -1;
				}
			}
			else if (token == "-c") {
				check = true;
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
				std::cout << "Unknown command line option: " << token << std::endl;
				help();
				return -1;
			}
		}
		
		if (precision < 0) {
			precision = 32;
		}
		
		if (! (snapType & (ProjectSN::ST_PLANE|ProjectSN::ST_SPHERE))) {
			snapType |= ProjectSN::ST_PLANE;
		}
		
		if (! (snapType & (ProjectSN::ST_CF|ProjectSN::ST_FX|ProjectSN::ST_FL))) {
			snapType |= ProjectSN::ST_FX;
		}
		
		return 1;
	}
};

std::ostream & operator<<(std::ostream & out, const Config & cfg) {
	out << "Precision: " << cfg.precision << '\n';
	out << "Float conversion method: ";
	if (cfg.snapType & ratss::ProjectSN::ST_CF) {
		out << "continous fraction";
	}
	else if (cfg.snapType & ratss::ProjectSN::ST_FX) {
		out << "fix point";
	}
	else if (cfg.snapType & ratss::ProjectSN::ST_FL) {
		out << "floating point";
	}
	out << '\n';
	out << "Float conversion location: " << (cfg.snapType & ratss::ProjectSN::ST_SPHERE ? "sphere" : "plane") << '\n';
	out << "Check: " << (cfg.check ? "yes" : "no") << '\n';
	out << "Normalize: " << (cfg.normalize ? "yes" : "no") << '\n';
	out << "Output format: ";
	if (cfg.outFormat == OutputPoint::FM_FLOAT) {
		out << "float";
	}
	else if (cfg.outFormat == OutputPoint::FM_RATIONAL) {
		out << "rational";
	}
	else if (cfg.outFormat == OutputPoint::FM_SPLIT_RATIONAL) {
		out << "rational split by space";
	}
	out << '\n';
	out << "Input file: " << (cfg.inFileName.size() ? cfg.inFileName : "stdin") << '\n';
	out << "Output file: " << (cfg.outFileName.size() ? cfg.outFileName : "stdout") << '\n';
	return out;
}

int main(int argc, char ** argv) {
	Config cfg;
	ProjectSN proj;
	ratss::BitCount bc;

	int ret = cfg.parse(argc, argv); 
	
	if (ret <= 0) {
		return ret;
	}
	
	if (cfg.verbose) {
		std::cout << cfg << std::endl;
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
	
	while( inFile->good() ) {
		for( ; inFile->good() && inFile->peek() == '\n'; inFile->get()) {}
		if (!inFile->good()) {
			break;
		}
		ip.assign(*inFile);
		if (cfg.normalize) {
			if (cfg.verbose) {
				std::cout << "Normalizing (" << ip << ") to ";
			}
			ip.normalize();
			if (cfg.verbose) {
				std::cout << '(' << ip << ')' << '\n';
			}
		}
		ip.setPrecision(cfg.precision);
		op.clear();
		op.resize(ip.coords.size());
		proj.snap(ip.coords.begin(), ip.coords.end(), op.coords.begin(), cfg.snapType);
		if (cfg.stats) {
			bc.update(op.coords.begin(), op.coords.end());
		}
		if (cfg.check && !op.valid()) {
			std::cerr << "Invalid projection for point " << ip << std::endl;
			return -1;
		}
		op.print(*outFile, cfg.outFormat);
		*outFile << '\n';
	}
	
	if (cfg.stats) {
		std::cout << bc << std::endl;
	}
	
	
	return 0;
}