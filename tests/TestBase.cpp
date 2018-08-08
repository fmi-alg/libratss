#include "TestBase.h"


extern "C" {
	
void debug_print_mpreal(const mpfr::mpreal & v) {
	std::cerr << v << std::endl;
}

void debug_print_mpq_class(const mpq_class & v) {
	std::cerr << v << std::endl;
}

void debug_print_mpz_class(const mpz_class & v) {
	std::cerr << v << std::endl;
}

}


namespace LIB_RATSS_NAMESPACE {
namespace tests {

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
