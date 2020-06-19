#include <libratss/util/BasicCmdLineOptions.h>
#include <libratss/ProjectSN.h>

namespace LIB_RATSS_NAMESPACE {

BasicCmdLineOptions::SnapTypeHelper::SnapTypeHelper() {

#define ENTRY(__NAME, __SHORT, __DESC) \
	m_st.push_back(ST_ ## __NAME); \
	m_st2str[ST_ ## __NAME] = #__NAME; \
	m_str2st[#__NAME] = ST_ ## __NAME; \
	m_str2st["ST_" #__NAME] = ST_ ## __NAME; \
	m_desc[ST_ ## __NAME] = __DESC;

	ENTRY(AUTO_ALL, "AUTO-ALL", "Try all snapping types")
	ENTRY(AUTO, "AUTO-ALL", "Enable auto selection of snap type")
	ENTRY(AUTO_CF, "ACF", "Add cf to auto selection")
	ENTRY(AUTO_FX, "AFX", "Add fx to auto selection")
	ENTRY(AUTO_FL, "AFL", "Add fl to auto selection")
	ENTRY(AUTO_JP, "AJP", "Add jp to auto selection")
	ENTRY(AUTO_FPLLL_FIXED_N, "ALLL-FN", "Add fplll with fixed N to auto selection")
	ENTRY(AUTO_FPLLL,, "ALLL" "Add fplll to auto selection")
	ENTRY(AUTO_BRUTE_FORCE, "ABF", "Add brute-force to auto selection")
	ENTRY(AUTO_POLICY_MIN_SUM_DENOM, "MSD", "Set auto snap policy")
	ENTRY(AUTO_POLICY_MIN_MAX_DENOM, "MMD", "Set auto snap policy")
	ENTRY(AUTO_POLICY_MIN_TOTAL_LIMBS, "MTL", "Set auto snap policy")
	ENTRY(AUTO_POLICY_MIN_SQUARED_DISTANCE, "MD2", "Set auto snap policy")
	ENTRY(AUTO_POLICY_MIN_MAX_NORM, "MN", "Set auto snap policy")
	ENTRY(CF, "CF", "Use continous fraction for snapping")
	ENTRY(FX, "FX", "Use fix-point for snapping")
	ENTRY(FL, "FL", "Use floating point for snapping")
	ENTRY(JP, "JP", "Use jacobi perron for snapping")
	ENTRY(FPLLL_FIXED_N, "LLL-FN", "Use fplll with fixed N for snapping")
	ENTRY(FPLLL, "LLL", "Use fplll for snapping")
	ENTRY(BRUTE_FORCE, "BF", "Use brute-force for snapping")
	ENTRY(INPUT_IS_EXACT, "E", "Input is exact and does not have an error")
	ENTRY(GUARANTEE_DISTANCE, "GD", "Guarantee distance, try to minimize size of the denominator")
	ENTRY(GUARANTEE_SIZE, "GS", "Guarantee maximum size of denominator, try to minimize distance to input")
	ENTRY(SPHERE, "S", "Snap on sphere")
	ENTRY(PLANE, "P", "Snap in the plane")
	ENTRY(PAPER, "PAPER", "Use Core::Expr to correctly scale down points to the plane.")
	ENTRY(PAPER2, "PAPER", "Use Core2::Expr to correctly scale down point to the plane. Also support geo points as input.")
	ENTRY(NORMALIZE, "N", "Normalize input to 1 before snapping.")
#undef ENTRY
}

std::string
BasicCmdLineOptions::SnapTypeHelper::description(SnapType st) const {
	if (m_desc.count(st)) {
		return m_desc.at(st);
	}
	else {
		return "No description available";
	}
}

std::string
BasicCmdLineOptions::SnapTypeHelper::toString(int st) const {
	if (st == ST_NONE) {
		return "ST_NONE";
	}
	//first take care of compound flags
	std::string result;
	for(int x : m_st) {
		if ((st & x) == x) {
			result += "|" + m_st2str.at(x);
			st &= ~x;
		}
	}
	if (!result.size()) {
		return "INVALID";
	}
	return std::string(result.begin()+1, result.end());
}

int
BasicCmdLineOptions::SnapTypeHelper::fromString(std::string const & str) const {
	int result = 0;
	std::string token;
	for(char c : str) {
		if (c == ' ') {
			continue;
		}
		else  if (c == '|' || c == ',') {
			if (m_str2st.count(token)) {
				result |= m_str2st.at(token);
			}
			else {
				throw std::runtime_error("Invalid token: " + token);
			}
		}
		else if (c == '_') {
			token += c;
		}
		else if ('A' <= c && c <= 'Z') {
			token += c;
		}
		else if ('a' <= c && c <= 'z') {
			token += c-32;
		}
	}
	return result;
}

BasicCmdLineOptions::BasicCmdLineOptions() {}

BasicCmdLineOptions::~BasicCmdLineOptions() {}

int BasicCmdLineOptions::parse(int argc, char ** argv) {
	int numParsedOpts = 0;
	for(int i(1); i < argc; ++i) {
		std::string token(argv[i]);
		if (token == "-c") {
			if (i+1 < argc) {
				precision = ::atoi(argv[i+1]);
				++i;
			}
			else {
				return -1;
			}
		}
		else if (token == "-p") {
			if (i+1 < argc) {
				significands = ::atoi(argv[i+1]);
				boundMode = BM_SIGNIFICANDS;
				++i;
			}
			else {
				return -1;
			}
		}
		else if (token == "-Q") {
			if  (i+1 < argc) {
				maxDen = mpz_class(std::string(argv[i+1]));
				boundMode = BM_MAX_DEN;
				++i;
			}
			else {
				return -1;
			}
		}
		else if (token == "-e") {
			if  (i+1 < argc) {
				epsilon = abs(mpq_class(argv[i+1]));
				boundMode = BM_EPSILON;
				++i;
			}
			else {
				return -1;
			}
		}
		else if (token == "-s") {
			if (i+1 < argc) {
				snapType = m_sth.fromString(argv[i+1]);
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
		"General options:\n"
		"\t--verbose\tverbose\n"
		"\t--progress\tprogress indicators\n"

		"\nComputation options\n"
		"\t-c num\tset the precision of the input and the precision of subsequent computations in bits.\n"
		"\t-p k\tset significands to k which translates to an epsilon of 2^-k\n"
		"\t-Q num\tset the largest possible denominator. Only useful for brute force and lll modes.\n"
		"\t-e rational\tset a specific epsilon given as a rational.\n"
		"\nInput options:\n"
		"\t--rational-pass-through\t don't snap rational input coordinates\n"
		"\t-if format\tset input format: [spherical, geo, cartesian=[rational, split, float, float128]]\n"
		"\t-i\tpath to input\n"
		"\t--stats (sum|each|bits|distance)\tCompute statistics for all points (sum) or each point.\n"
		"\t-of format\tset output format: [spherical, geo, rational, split, float, float128]\n"
		"\t-o\tpath to output\n"
		"-s snap type flags: \n";
	for(auto st : m_sth.types()) {
		out << m_sth.toString(st) << ": " << m_sth.description(SnapType(st)) << '\n';
	}
}

void BasicCmdLineOptions::options_selection(std::ostream& out) const {
	out << "Precision: " << precision << '\n';
	if (significands > 0) {
		out << "Significands: " << significands << '\n';
	}
	out << "Snap type: " << m_sth.toString(snapType);
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
