#include <libratss/util/BasicCmdLineOptions.h>
#include <libratss/ProjectSN.h>

namespace LIB_RATSS_NAMESPACE {

BasicCmdLineOptions::BasicCmdLineOptions() :
precision(-1),
significands(-1),
snapType(ST_NONE),
normalize(false),
verbose(false),
progress(false),
rationalPassThrough(false),
inFormat(FloatPoint::FM_CARTESIAN_FLOAT),
outFormat(RationalPoint::FM_RATIONAL)
{}

BasicCmdLineOptions::~BasicCmdLineOptions() {}

int BasicCmdLineOptions::parse(int argc, char ** argv) {
	int numParsedOpts = 0;
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
				significands = ::atoi(argv[i+1]);
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
					snapType |= ST_CF;
				}
				else if (stStr == "fx") {
					snapType |= ST_FX;
				}
				else if (stStr == "fl") {
					snapType |= ST_FL;
				}
				else if ( stStr == "jp") {
					snapType |= ST_JP;
				}
				else if ( stStr == "lll") {
					snapType |= ST_FPLLL;
				}
				else if ( stStr == "lllg") {
					snapType |= ST_FPLLL_GREEDY;
				}
				else if ( stStr == "bf") {
					snapType |= ST_BRUTE_FORCE;
				}
				else if (stStr == "ml" || stStr == "minlimb") {
					snapType |= ST_AUTO_ALL | ST_AUTO_POLICY_MIN_TOTAL_LIMBS;
				}
				else if (stStr == "md" || stStr == "mindenom") {
					snapType |= ST_AUTO_ALL | ST_AUTO_POLICY_MIN_MAX_DENOM;
				}
				else if (stStr == "msd" || stStr == "minsumdenom") {
					snapType |= ST_AUTO_ALL | ST_AUTO_POLICY_MIN_SUM_DENOM;
				}
				else if (stStr == "md2" || stStr == "minsqdist") {
					snapType |= ST_AUTO_ALL | ST_AUTO_POLICY_MIN_SQUARED_DISTANCE;
				}
				else if (stStr == "mmn" || stStr == "minmaxnorm") {
					snapType |= ST_AUTO_ALL | ST_AUTO_POLICY_MIN_MAX_NORM;
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
					snapType &= ~ST_SPHERE;
					snapType |= ST_PLANE;
				}
				else if (stStr == "s" || stStr == "sphere") {
					snapType |= ST_SPHERE;
					snapType &= ~ST_PLANE;
				}
				else if (stStr == "paper") {
					snapType |= ST_PAPER;
					snapType &= ~ST_PAPER;
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
				if (stStr == "geo" || stStr == "g") {
					inFormat = FloatPoint::FM_GEO;
				}
				else if (stStr == "spherical" || stStr == "sl") {
					inFormat = FloatPoint::FM_SPHERICAL;
				}
				else if (stStr == "cartesian" || stStr == "c" || stStr == "float" || stStr == "f" || stStr == "float128" || stStr == "f128") {
					inFormat = FloatPoint::FM_CARTESIAN_FLOAT;
				}
				else if (stStr == "rational" || stStr == "r") {
					inFormat = FloatPoint::FM_CARTESIAN_RATIONAL;
				}
				else if (stStr == "split" || stStr == "sr") {
					inFormat = FloatPoint::FM_CARTESIAN_SPLIT_RATIONAL;
				}
				else {
					std::cerr << "Unrecognized input format: " << stStr << std::endl;
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
					outFormat = RationalPoint::FM_RATIONAL;
				}
				else if (stStr == "split" || stStr == "sr") {
					outFormat = RationalPoint::FM_SPLIT_RATIONAL;
				}
				else if (stStr == "float" || stStr == "double" || stStr == "d" || stStr == "f") {
					outFormat = RationalPoint::FM_FLOAT;
				}
				else if (stStr == "float128" || stStr == "f128") {
					outFormat = RationalPoint::FM_FLOAT128;
				}
				else if (stStr == "geo" || stStr == "g") {
					outFormat = RationalPoint::FM_GEO;
				}
				else if (stStr == "spherical" || stStr == "sl") {
					outFormat = RationalPoint::FM_SPHERICAL;
				}
				else {
					std::cerr << "Unrecognized output format: " << stStr << std::endl;
				}
				++i;
			}
			else {
				return -1;
			}
		}
		else if (token == "-n") {
			normalize = true;
		}
		else if (token == "-v" || token == "--verbose") {
			verbose = true;
		}
		else if (token == "--progress") {
			progress = true;
		}
		else if (token == "--rational-pass-through") {
			rationalPassThrough = true;
		}
		else if (token == "-h" || token == "--help") {
			return 0;
		}
		else {
			bool parseOk = false;
			try {
				parseOk = this->parse(token, i, argc, argv);
			}
			catch (const ParseError & e) {
				std::cerr << e.what() << std::endl;
				return -1;
			}
			
			if (!parseOk) {
				std::cerr << "Unknown command line option: " << token << std::endl;
				return -1;
			}
		}
		++numParsedOpts;
	}
	
	if (snapType & (ST_PAPER|ST_PAPER2) && normalize) {
		std::cerr << "Requesting normaliziation of input with paper snapping is of no use." << std::endl;
		return -1;
	}
	
	if (precision < 0) {
		precision = 53;
	}
	
	if (significands < 0) {
		significands = 31;
	}
	
	if ((snapType & ST_SNAP_POSITION_MASK) == 0) {
		snapType |= ST_PLANE;
	}
	
	if ((snapType & ST_SNAP_TYPES_MASK) == 0) {
		snapType |= ST_FX;
	}
	
	parse_completed();
	
	return numParsedOpts;
}

bool BasicCmdLineOptions::parse(const std::string & /*currentToken*/,int & /*i*/, int /*argc*/, char ** /*argv*/) {
	return false;
}

void BasicCmdLineOptions::parse_completed() {}

void BasicCmdLineOptions::options_help(std::ostream& out) const {
	out <<
		"\t-v\tverbose\n"
		"\t-e k\tset significands to k which translates to an epsilon of 2^-k\n"
		"\t-p num\tset the precision of the input in bits\n"
		"\t-r (cf|fl|fx|jp|lll|lllg|bf)\tset the type of float->rational conversion. fx=fixpoint, cf=continous fraction, fl=floating point, jp=jacobi-perron, lll=LLL, lllg=LLL-greedy, bf=bruteforce, ml=minlimb, md=mindenom, msd=minsumdenom, md2=minsqdist, mmn=minmaxnorm\n"
		"\t-s (s=sphere|p=plane|paper)\tset where the float->rational conversion should take place.\n"
		"\t-n\tnormalize input to length 1\n"
		"\t--progress\tprogress indicators\n"
		"\t--rational-pass-through\t don't snap rational input coordinates\n"
		"\t-if format\tset input format: [spherical, geo, cartesian=[rational, split, float, float128]]\n"
		"\t-of format\tset output format: [spherical, geo, rational, split, float, float128]\n"
		"\t-i\tpath to input\n"
		"\t-o\tpath to output";
}

void BasicCmdLineOptions::options_selection(std::ostream& out) const {
	out << "Precision: " << precision << '\n';
	if (significands > 0) {
		out << "Significands: " << significands << '\n';
	}
	out << "Float conversion method: ";
	if (snapType & ratss::ST_CF) {
		out << "continous fraction";
	}
	else if (snapType & ratss::ST_FX) {
		out << "fix point";
	}
	else if (snapType & ratss::ST_FL) {
		out << "floating point";
	}
	else if (snapType & ratss::ST_JP) {
		out << "jacobi-perron";
	}
	else if (snapType & ratss::ST_FPLLL) {
		out << "LLL";
	}
	else if (snapType & ratss::ST_FPLLL_GREEDY) {
		out << "LLL Greedy";
	}
	else if (snapType & ratss::ST_BRUTE_FORCE) {
		out << "Brute force";
	}
	else if (snapType & ratss::ST_AUTO_POLICY_MIN_MAX_DENOM) {
		out << "minimize maximum denominator";
	}
	else if (snapType & ratss::ST_AUTO_POLICY_MIN_SUM_DENOM) {
		out << "minimize summed denominators";
	}
	else if (snapType & ratss::ST_AUTO_POLICY_MIN_TOTAL_LIMBS) {
		out << "minimize total number of limbs";
	}
	else if (snapType & ratss::ST_AUTO_POLICY_MIN_SQUARED_DISTANCE) {
		out << "minimize squared distance to input point";
	}
	else if (snapType & ratss::ST_AUTO_POLICY_MIN_MAX_NORM) {
		out << "minimize maximum norm";
	}
	else {
		out << "invalid";
	}
	out << '\n';
	out << "Float conversion location: " << (snapType & ratss::ST_SPHERE ? "sphere" : "plane") << '\n';
	out << "Normalize: " << (normalize ? "yes" : "no") << '\n';
	out << "Input format: ";
	if (inFormat == FloatPoint::FM_GEO) {
		out << "geo";
	}
	else if (inFormat == FloatPoint::FM_SPHERICAL) {
		out << "spherical";
	}
	else if (inFormat == FloatPoint::FM_CARTESIAN_FLOAT) {
		out << "cartesian floating point";
	}
	else if (inFormat == FloatPoint::FM_CARTESIAN_RATIONAL) {
		out << "cartesian rational";
		if (rationalPassThrough) {
			out << " pass-through";
		}
	}
	else if (inFormat == FloatPoint::FM_CARTESIAN_SPLIT_RATIONAL) {
		out << "cartesian split rational";
		if (rationalPassThrough) {
			out << " pass-through";
		}
	}
	out << '\n';
	out << "Output format: ";
	if (outFormat == RationalPoint::FM_FLOAT) {
		out << "float";
	}
	else if (outFormat == RationalPoint::FM_RATIONAL) {
		out << "rational";
	}
	else if (outFormat == RationalPoint::FM_SPLIT_RATIONAL) {
		out << "rational split by space";
	}
	out << '\n';
	out << "Input file: " << (inFileName.size() ? inFileName : "stdin") << '\n';
	out << "Output file: " << (outFileName.size() ? outFileName : "stdout");
}


}//end namespace LIB_RATSS_NAMESPACE
