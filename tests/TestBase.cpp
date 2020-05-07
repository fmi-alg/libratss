#include "TestBase.h"

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
