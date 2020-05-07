# This is the build file to build libratss
# It can be build as a stand-alone project as well as a sub-project
# It has support for imported targets
# You may simply do the following:
# 
# add_subdirectory(<path to ratss>)
# target_link_libraries(<target> ratss)

cmake_minimum_required(VERSION 3.9)
project(ratss)
enable_testing()

set(CMAKE_MODULE_PATH
	${CMAKE_CURRENT_SOURCE_DIR}/cmake
)

include(add_buildtype_lto)
include(add_buildtype_lto_sanitize)
include(add_buildtype_sanitize_debug)

find_package(Threads)
find_package(LIBGMPXX REQUIRED)
find_package(LIBMPFR REQUIRED)
find_package(PkgConfig)

if (NOT LIBRATSS_DISABLE_CGAL_SUPPORT)
	find_package(CGAL COMPONENTS Core)
endif(NOT LIBRATSS_DISABLE_CGAL_SUPPORT)

if (CGAL_FOUND AND CGAL_Core_FOUND)
	set(LIBRATSS_CGAL_LINK_LIBRARIES
		CGAL::CGAL
		CGAL::CGAL_Core
	)
	set(LIBRATSS_COMPILE_DEFINITIONS
		${LIBRATSS_COMPILE_DEFINITIONS}
		"LIB_RATSS_WITH_CGAL=1"
	)
	message(STATUS "Enabling support for CGAL")
else()
	message(STATUS "Disabling support for CGAL")
endif()

if (PKGCONFIG_FOUND AND NOT LIBRATSS_DISABLE_FPLLL_SUPPORT)
	pkg_search_module(FPLLL fplll)
endif()

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/vendor/core2/CMakeLists.txt" AND NOT LIBRATSS_DISABLE_CORE_TWO_SUPPORT)
	add_subdirectory(vendor/core2 ${PROJECT_NAME}core2)
	
	set(LIBRATSS_COMPILE_DEFINITIONS
		${LIBRATSS_COMPILE_DEFINITIONS}
		"LIB_RATSS_WITH_CORE_TWO=1"
	)
	set(CORE_TWO_FOUND TRUE)
	set(CORE_TWO_LIBRARIES core2)
	message(STATUS "Enabling support for core2")
else()
	message(STATUS "Disabling support for core2")
endif()

set(LIBRATSS_WITH_VERBOSE_DEBUG OFF CACHE BOOL "Enable verbose debugging")

if (LIBRATSS_WITH_VERBOSE_DEBUG)
	set(LIBRATSS_COMPILE_DEFINITIONS
		${LIBRATSS_COMPILE_DEFINITIONS}
		"LIBRATSS_DEBUG_VERBOSE=1"
	)
endif()

if (FPLLL_FOUND)
	set(LIBRATSS_COMPILE_DEFINITIONS
		${LIBRATSS_COMPILE_DEFINITIONS}
		"LIB_RATSS_WITH_FPLLL=1"
	)
	set(LIBRATSS_COMPILE_OPTIONS
		${LIBRATSS_COMPILE_OPTIONS}
		${FPLLL_CFLAGS_OTHER}
	)
	message(STATUS "Enabling support for fplll")
else(FPLLL_FOUND)
	message(STATUS "Disabling support for fplll")
endif(FPLLL_FOUND)

set(LIBRATSS_INCLUDE_DIR
	"${CMAKE_CURRENT_SOURCE_DIR}/include"
	${LIBGMP_INCLUDE_DIR}
	${LIBGMPXX_INCLUDE_DIR}
	${LIBMPFR_INCLUDE_DIR}
	${FPLLL_INCLUDE_DIRS}
)

set(LIBRATSS_LINK_LIBRARIES
	${LIBRATSS_CGAL_LINK_LIBRARIES}
	${CMAKE_THREAD_LIBS_INIT}
	${LIBGMPXX_LIBRARIES}
	${LIBMPFR_LIBRARIES}
	${FPLLL_LIBRARIES}
	${CORE_TWO_LIBRARIES}
)

set(LIB_SOURCES_CPP
	src/Conversion.cpp
	src/ProjectSN.cpp
	src/ProjectS2.cpp
	src/Calc.cpp
	src/GeoCalc.cpp
	src/GeoCoord.cpp
	src/SphericalCoord.cpp
	src/SimApxBruteForce.cpp
	src/util/BasicCmdLineOptions.cpp
	src/util/InputOutputPoints.cpp
	src/util/InputOutput.cpp
)

if (CGAL_FOUND)
	set(LIB_SOURCES_CPP
		${LIB_SOURCES_CPP}
		src/CGAL/Conversion.cpp
		src/CGAL/ExtendedInt64z.cpp
		src/CGAL/ExtendedInt64q.cpp
		src/CGAL/boost_int1024q_traits.cpp
	)
endif(CGAL_FOUND)

if (FPLLL_FOUND)
	set(LIB_SOURCES_CPP
		${LIB_SOURCES_CPP}
		src/SimApxLLL.cpp
	)
endif(FPLLL_FOUND)

if (CORE_TWO_FOUND)
	set(LIB_SOURCES_CPP
		${LIB_SOURCES_CPP}
		src/core2/Conversion.cpp
	)
endif()
add_library(${PROJECT_NAME} STATIC
	${LIB_SOURCES_CPP}
)
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_14)
target_include_directories(${PROJECT_NAME} PUBLIC ${LIBRATSS_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} PUBLIC ${LIBRATSS_LINK_LIBRARIES})
target_compile_definitions(${PROJECT_NAME} PUBLIC ${LIBRATSS_COMPILE_DEFINITIONS})
target_compile_options(${PROJECT_NAME} PUBLIC ${LIBRATSS_COMPILE_OPTIONS})
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra)

add_subdirectory(common ${PROJECT_NAME}common EXCLUDE_FROM_ALL)
add_subdirectory(bench ${PROJECT_NAME}bench EXCLUDE_FROM_ALL)
add_subdirectory(tests ${PROJECT_NAME}tests EXCLUDE_FROM_ALL)
add_subdirectory(tools ${PROJECT_NAME}tools EXCLUDE_FROM_ALL)
