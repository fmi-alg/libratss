cmake_minimum_required(VERSION 3.9)
project(ratsstests)
find_package(CPPUNIT)

if (CPPUNIT_FOUND)

set(TEST_LIB_SOURCES_CPP
	TestBase.cpp
)

add_library(${PROJECT_NAME} STATIC ${TEST_LIB_SOURCES_CPP})
target_link_libraries(${PROJECT_NAME} ratss ratsscommon ${CPPUNIT_LIBRARIES})

MACRO(ADD_TEST_TARGET_SINGLE _name)
	add_executable("${PROJECT_NAME}_${_name}" "${_name}.cpp")
	target_link_libraries("${PROJECT_NAME}_${_name}" ratsstests)
	set(RATSSTESTS_ALL_TARGETS ${RATSSTESTS_ALL_TARGETS} "${PROJECT_NAME}_${_name}")
	add_test(NAME "${PROJECT_NAME}_${_name}" COMMAND "${PROJECT_NAME}_${_name}")
ENDMACRO(ADD_TEST_TARGET_SINGLE)

#containers
ADD_TEST_TARGET_SINGLE(projection)
ADD_TEST_TARGET_SINGLE(nd_projection)
ADD_TEST_TARGET_SINGLE(calc)
ADD_TEST_TARGET_SINGLE(compilation)

add_custom_target(${PROJECT_NAME}_all DEPENDS ${RATSSTESTS_ALL_TARGETS})

else()
	message(WARNING "Could not find cppunit library needed to compile the tests")
endif()
