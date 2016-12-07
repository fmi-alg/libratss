#include <libratss/constants.h>
#include <libratss/Calc.h>

#include "TestBase.h"
#include "../common/generators.h"

namespace LIB_RATSS_NAMESPACE {
namespace tests {

class CalcTest: public TestBase {
CPPUNIT_TEST_SUITE( CalcTest );
CPPUNIT_TEST( withinSpecial );
CPPUNIT_TEST_SUITE_END();
public:
	static std::size_t num_random_test_points;
public:
	Calc calc;
public:
	void withinSpecial();
};

std::size_t CalcTest::num_random_test_points;

}} // end namespace ratss::tests

int main(int argc, char ** argv) {
	LIB_RATSS_NAMESPACE::tests::TestBase::init(argc, argv);
	LIB_RATSS_NAMESPACE::tests::CalcTest::num_random_test_points = 10000;
	srand( 0 );
	CppUnit::TextUi::TestRunner runner;
	runner.addTest(  LIB_RATSS_NAMESPACE::tests::CalcTest::suite() );
	bool ok = runner.run();
	return ok ? 0 : 1;
}

namespace LIB_RATSS_NAMESPACE {
namespace tests {

void CalcTest::withinSpecial() {
	mpq_class lower, upper, within;
	
	lower = mpq_class("7/16");
	upper = mpq_class("9/16");
	within = calc.within(lower, upper);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("[7/16, 8/16]", mpq_class("1/2"), within);
}

}} //end namespace ratss::tests