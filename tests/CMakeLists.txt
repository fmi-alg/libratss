cmake_minimum_required(VERSION 2.8)
project(ratsstests)
find_package(CPPUNIT REQUIRED)

include_directories(
	${PROJECT_GLOBAL_SOURCE_DIR}
	${PROJECT_GLOBAL_SOURCE_DIR}/include
	${CMAKE_CURRENT_BINARY_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_SOURCE_DIR}
	${CMAKE_SOURCE_DIR}/include
)

set(TEST_LIB_SOURCES_CPP
	TestBase.cpp
)

add_library(${PROJECT_NAME} STATIC ${TEST_LIB_SOURCES_CPP})
add_dependencies(${PROJECT_NAME} ratss ratsscommon)

MACRO(ADD_TEST_TARGET_SINGLE _name)
	add_executable("${PROJECT_NAME}_${_name}" "${_name}.cpp")
	target_link_libraries("${PROJECT_NAME}_${_name}" ${TEST_LIB_LINK_LIBS})
	add_dependencies("${PROJECT_NAME}_${_name}" ratsstests)
ENDMACRO(ADD_TEST_TARGET_SINGLE)

set(TEST_LIB_LINK_LIBS
	${ratsstests_BINARY_DIR}/libratsstests.a
	${ratss_BINARY_DIR}/libratss.a
	${ratsscommon_BINARY_DIR}/libratsscommon.a
	${CPPUNIT_LIBRARY}
	"CGAL"
	"CGAL_Core"
	"gmpxx"
	"mpfr"
	"gmp"
	"boost_thread"
	"boost_system"
	"pthread"
)

#containers
ADD_TEST_TARGET_SINGLE(projection)
ADD_TEST_TARGET_SINGLE(nd_projection)
ADD_TEST_TARGET_SINGLE(calc)