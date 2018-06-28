#include <libratss/constants.h>
#include <libratss/ProjectS2.h>
#include <libratss/Conversion.h>

#include "TestBase.h"

namespace LIB_RATSS_NAMESPACE {
namespace tests {

class CompilationTest: public TestBase {
CPPUNIT_TEST_SUITE( CompilationTest );
CPPUNIT_TEST( conversion );
CPPUNIT_TEST_SUITE_END();
public:
	void conversion();
};

}} // end namespace ratss::tests

int main(int argc, char ** argv) {
	LIB_RATSS_NAMESPACE::tests::TestBase::init(argc, argv);
	srand( 0 );
	CppUnit::TextUi::TestRunner runner;
	runner.addTest(  LIB_RATSS_NAMESPACE::tests::CompilationTest::suite() );
	bool ok = runner.run();
	return ok ? 0 : 1;
}

namespace LIB_RATSS_NAMESPACE {
namespace tests {

void CompilationTest::conversion() {
	mpq_class x(0), y(1);
	y = Conversion<mpq_class>::moveFrom(x);
	CPPUNIT_ASSERT(x == y);
}

}} //end namespace LIB_RATSS_NAMESPACE::tests
