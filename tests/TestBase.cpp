#include "TestBase.h"

extern "C" {
	
void debug_print_mpreal(const mpfr::mpreal & v) {
	mpfr_exp_t exp;
	char * binstr = ::mpfr_get_str(0, &exp, 2, 0, v.mpfr_ptr(), MPFR_RNDZ);
	std::cerr << "value=" << v << " precision=" << v.get_prec() << " exponent=" << v.get_exp() << " binary=" <<  binstr << "E" << exp << std::endl;
	mpfr_free_str(binstr);
}

void debug_print_mpq_class(const mpq_class & v) {
	using LIB_RATSS_NAMESPACE::tests::numBits;
	std::cerr << "value=" << v << "~" << v.get_d();
	std::cerr << " binary=" << v.get_str(2);
	std::cerr << " bits=" << numBits(v.get_num()) << "/" << numBits(v.get_den());
	std::cerr << std::endl;
}

void debug_print_mpz_class(const mpz_class & v) {
	std::cerr << "value=" << v << " bits=" << LIB_RATSS_NAMESPACE::tests::numBits(v) << std::endl;
}

void debug_print_mpf_class(const mpf_class & v) {
	std::cerr << "value=" << v << " precision=" << v.get_prec() << " exp=" << v.get_ui() << std::endl;
}

void debug_print_CORE_BigFloat(CORE::BigFloat const & v) {
	std::cerr << "value=" << v << " m=" << mpz_class(v.m().get_mp()) << " exp=" << v.exp() << std::endl;
}

}


namespace LIB_RATSS_NAMESPACE {
namespace tests {
	
std::size_t numBits(mpq_class const & v) {
	return std::max(numBits(v.get_num()), numBits(v.get_den()));
}

std::size_t numBits(mpz_class const & v) {
	return mpz_sizeinbase(v.get_mpz_t(), 2);
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
