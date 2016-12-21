#include <libratss/constants.h>
#include <libratss/Calc.h>

#include "TestBase.h"
#include "../common/generators.h"

namespace LIB_RATSS_NAMESPACE {
namespace tests {

class CalcTest: public TestBase {
CPPUNIT_TEST_SUITE( CalcTest );
// CPPUNIT_TEST( withinSpecial );
// CPPUNIT_TEST( contFracRandom );
CPPUNIT_TEST( jacobiPerron2D );
CPPUNIT_TEST_SUITE_END();
public:
	static std::size_t num_random_test_points;
public:
	Calc calc;
public:
	void withinSpecial();
	void contFracRandom();
	void jacobiPerron2D();
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

void CalcTest::contFracRandom() {
	;
}

void CalcTest::jacobiPerron2D() {
	mpq_class input1("3/16");
	mpq_class input2("3/11");
	
	mpq_class output1, output2;
	
	calc.jacobiPerron2D(input1, input2, output1, output2, 16);
	
	CPPUNIT_ASSERT_EQUAL(input1, output1);
	CPPUNIT_ASSERT_EQUAL(input2, output2);
}

void CalcTest::withinSpecial() {
	mpq_class lower, upper, within;
	std::stringstream ss;
	
	lower = mpq_class("7/16");
	upper = mpq_class("9/16");
	within = calc.within(lower, upper);
	ss << "[" << lower << ", " << upper << "] -> " << within;
	CPPUNIT_ASSERT_EQUAL_MESSAGE(ss.str(), mpq_class("1/2"), within);
	ss.clear();
	
	lower = mpq_class("616882175/4294967296");
	upper = mpq_class("75303/524288");
	within = calc.within(lower, upper);
	ss << "[" << lower << ", " << upper << "] -> " << within;
	CPPUNIT_ASSERT_MESSAGE(ss.str(), within >= lower);
	CPPUNIT_ASSERT_MESSAGE(ss.str(), within <= upper);
	ss.clear();

	lower = mpq_class("93199540/290310467");
	upper = mpq_class("93199540/290310467");
	within = calc.within(lower, upper);
	ss << "[" << lower << ", " << upper << "] -> " << within;
	CPPUNIT_ASSERT(lower <= upper);
	CPPUNIT_ASSERT_MESSAGE(ss.str(), within >= lower);
	CPPUNIT_ASSERT_MESSAGE(ss.str(), within <= upper);
	ss.clear();
	
	lower = mpq_class("86176917/268435456");
	upper = mpq_class("2891617501446145/9007199254740992");
	within = calc.within(lower, upper);
	ss << "[" << lower << ", " << upper << "] -> " << within;
	CPPUNIT_ASSERT(lower < upper);
	CPPUNIT_ASSERT_MESSAGE(ss.str(), within >= lower);
	CPPUNIT_ASSERT_MESSAGE(ss.str(), within <= upper);
	ss.clear();
}

}} //end namespace ratss::tests