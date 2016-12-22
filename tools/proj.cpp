#include <libratss/ProjectSN.h>
#include "../common/stats.h"
#include "../common/points.h"
#include <fstream>
#include "types.h"

using namespace LIB_RATSS_NAMESPACE;
using namespace LIB_RATSS_NAMESPACE::tools;

std::ostream & operator<<(std::ostream & out, const InputPoint & ip) {
	ip.print(out);
	return out;
}

void help(std::ostream & out) {
	out << "prg OPTIONS\n"
		"Options:\n"
		"\t-v\tverbose\n"
		"\t-p num\tset the precision of the input in bits\n"
		"\t-r (cf|fl|fx|jp)\tset the type of float->rational conversion. fx=fixpoint, cf=continous fraction, fl=floating point, jp=jacobi-perron\n"
		"\t-s (s|sphere|p|plane)\tset where the float->rational conversion should take place\n"
		"\t-b\talso print bitsize statistics\n"
		"\t-n\tnormalize input to length 1\n"
		"\t-if format\tset input format: [geo, spherical, cartesian]\n"
		"\t-of format\tset output format: [rational, split, float]\n"
		"\t-e k\tset manual epsilon to be 2^-k\n"
		"\t-i\tpath to input\n"
		"\t-o\tpath to output"
	<< std::endl;
}

struct Config {
	std::string inFileName;
	std::string outFileName;
	int precision;
	int eps;
	int snapType;
	bool normalize;
	bool stats;
	bool verbose;
	bool check;
	bool progress;
	InputPoint::Format inFormat;
	OutputPoint::Format outFormat;
	
	
	Config() :
	precision(-1),
	eps(-1),
	snapType(ProjectSN::ST_NONE),
	normalize(false),
	stats(false),
	verbose(false),
	check(false),
	progress(false),
	inFormat(InputPoint::FM_CARTESIAN),
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
					return -1;
				}
			}
			else if (token == "-e") {
				if (i+1 < argc) {
					eps = ::atoi(argv[i+1]);
					++i;
				}
				else {
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
					else if ( stStr == "jp") {
						snapType |= ProjectSN::ST_JP;
					}
					else {
						std::cerr << "Unrecognized snap method: " << stStr << std::endl;
						return -1;
					}
					++i;
				}
				else {
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
					else {
						std::cerr << "Unrecognized snap location: " << stStr << std::endl;
						return -1;
					}
					++i;
				}
				else {
					return -1;
				}
			}
			else if (token == "-i") {
				if (i+1 < argc) {
					inFileName.assign(argv[i+1]);
					++i;
				}
				else {
					std::cerr << "Unrecognized ";
					return -1;
				}
			}
			else if (token == "-o") {
				if (i+1 < argc) {
					outFileName.assign(argv[i+1]);
					++i;
				}
				else {
					return -1;
				}
			}
			else if (token == "-if") {
				if (i+1 < argc) {
					std::string stStr(argv[i+1]);
					if (stStr == "geo" ) {
						inFormat = InputPoint::FM_GEO;
					}
					else if (stStr == "spherical") {
						inFormat = InputPoint::FM_SPHERICAL;
					}
					else if (stStr == "cartesian") {
						inFormat = InputPoint::FM_CARTESIAN;
					}
					else {
						std::cerr << "Unrecognized input format: " << stStr << std::endl;
						help(std::cerr);
					}
					++i;
				}
				else {
					return -1;
				}
			}
			else if (token == "-of") {
				if (i+1 < argc) {
					std::string stStr(argv[i+1]);
					if (stStr == "rational" || stStr == "rat" || stStr == "r") {
						outFormat = OutputPoint::FM_RATIONAL;
					}
					else if (stStr == "split" || stStr == "sr" || stStr == "s") {
						outFormat = OutputPoint::FM_SPLIT_RATIONAL;
					}
					else if (stStr == "float" || stStr == "double" || stStr == "d" || stStr == "f") {
						outFormat = OutputPoint::FM_FLOAT;
					}
					else if (stStr == "float128" || stStr == "f128") {
						outFormat = OutputPoint::FM_FLOAT128;
					}
					else {
						std::cerr << "Unrecognized output format: " << stStr << std::endl;
						help(std::cerr);
					}
					++i;
				}
				else {
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
			else if (token == "--progress") {
				progress = true;
			}
			else if (token == "-h" || token == "--help") {
				return 0;
			}
			else {
				std::cerr << "Unknown command line option: " << token << std::endl;
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
	if (cfg.eps > 0) {
		out << "Epsilon: " << cfg.eps << '\n';
	}
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
	out << "Input format: ";
	if (cfg.inFormat == InputPoint::FM_GEO) {
		out << "geo";
	}
	else if (cfg.inFormat == InputPoint::FM_SPHERICAL) {
		out << "spherical";
	}
	else if (cfg.inFormat == InputPoint::FM_CARTESIAN) {
		out << "cartesian";
	}
	out << '\n';
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
		help(std::cerr);
		return ret;
	}
	
	if (cfg.verbose) {
		std::cerr << cfg << std::endl;
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
		proj.snap(ip.coords.begin(), ip.coords.end(), op.coords.begin(), cfg.snapType, cfg.eps);
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