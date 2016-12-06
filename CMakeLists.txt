cmake_minimum_required(VERSION 2.8)

set(CMAKE_MODULE_PATH
	${CMAKE_SOURCE_DIR}/cmake
	${CMAKE_CURRENT_SOURCE_DIR}/cmake
)

project(ratss)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++0x")

find_package(CGAL REQUIRED Core)

include_directories(
	${CMAKE_SOURCE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_SOURCE_DIR}/include
	${CMAKE_CURRENT_SOURCE_DIR}/include
)

get_directory_property(hasParent PARENT_DIRECTORY)
#export correct link libs and include dirs
if(hasParent)
	SET(LIBRATSS_INCLUDE_DIR
		"${CMAKE_CURRENT_SOURCE_DIR}/include"
		PARENT_SCOPE)
	
	SET(LIBRATSS_LIBRARIES
		"${CMAKE_CURRENT_BINARY_DIR}/libratss.a"
		PARENT_SCOPE)
	
	SET(LIBRATSS_LINK_LIBRARIES
		"${CMAKE_CURRENT_BINARY_DIR}/libratss.a"
		"CGAL"
		"CGAL_Core"
		"gmpxx"
		"mpfr"
		"gmp"
		"boost_thread"
		"boost_system"
		"pthread"
		PARENT_SCOPE
	)
endif()#hasParent

set(LIB_SOURCES_CPP
	src/Conversion.cpp
	src/ProjectS2.cpp
	src/Calc.cpp
	src/GeoCalc.cpp
	src/GeoCoord.cpp
	src/SphericalCoord.cpp
)

add_library(${PROJECT_NAME} STATIC
	${LIB_SOURCES_CPP}
)

add_subdirectory(common ${PROJECT_NAME}common EXCLUDE_FROM_ALL)
add_subdirectory(bench ${PROJECT_NAME}bench EXCLUDE_FROM_ALL)
add_subdirectory(tests ${PROJECT_NAME}tests EXCLUDE_FROM_ALL)
