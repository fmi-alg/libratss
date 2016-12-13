cmake_minimum_required(VERSION 2.8)
project(ratsstests)
find_package(CPPUNIT REQUIRED)

include_directories(
	${LIBRATSS_INCLUDE_DIR}
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

message("LIBRATSS_LINK_LIBRARIES=${LIBRATSS_LINK_LIBRARIES}")
set(TEST_LIB_LINK_LIBS
	ratsstests
	ratsscommon
	${LIBRATSS_LINK_LIBRARIES}
	${CPPUNIT_LIBRARIES}
)

#containers
ADD_TEST_TARGET_SINGLE(projection)
ADD_TEST_TARGET_SINGLE(nd_projection)
ADD_TEST_TARGET_SINGLE(calc)