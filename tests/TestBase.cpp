#include "TestBase.h"

extern "C" {
	
void debug_print_mpreal(const mpfr::mpreal & v) {
	mpfr_exp_t exp;
	char * binstr = ::mpfr_get_str(0, &exp, 2, 0, v.mpfr_ptr(), MPFR_RNDN);
	std::cerr << "value=" << v << " precision=" << v.get_prec() << " exponent=" << v.get_exp() << " binary=" <<  binstr << "E" << exp << std::endl;
	mpfr_free_str(binstr);
}

void debug_print_mpq_class(const mpq_class & v) {
	std::cerr << "value=" << v << " approximately " << v.get_d() << std::endl;
}

void debug_print_mpz_class(const mpz_class & v) {
	std::cerr << "value=" << v << std::endl;
}

}


namespace LIB_RATSS_NAMESPACE {
namespace tests {
	
std::size_t numBits(mpq_class const & v) {
	std::size_t sizeNum = mpz_sizeinbase(v.get_num().get_mpz_t(), 2);
	std::size_t sizeDenom = mpz_sizeinbase(v.get_den().get_mpz_t(), 2);
	return std::max(sizeNum, sizeDenom);
}

int TestBase::argc = 0;
char ** TestBase::argv = 0;

TestBase::TestBase() {
// 	if (!argc) {
// 		throw std::runtime_error("sserialize::tests::TestBase:: need to call init first!");
// 	}
}

TestBase::~TestBase() {}

void TestBase::init(int argc, char ** argv) {
	TestBase::argc = argc;
	TestBase::argv = argv;
}


}} //end namespace ratss::tests
