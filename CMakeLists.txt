# This is the build file to build libratss
# It can be build as a stand-alone project as well as a sub-project
# This build file exports the following variables into its parent scope:
# LIBRATSS_INCLUDE_DIR - The include directory
# LIBRATSS_LIBRARIES - libraries that need to be linker in order for libdts2 to work
# LIBRATSS_COMPILE_DEFINITIONS - definitions that are needed
# 
# You may simply do the following:
# 
# include_directories(${LIBRATSS_INCLUDE_DIR})
# 
# add_executable(${PROJECT_NAME} ${SOURCES_CPP}
# target_link_libraries(${PROJECT_NAME} ${LIBRATSS_LIBRARIES})
# add_dependencies(${PROJECT_NAME} ratss)

cmake_minimum_required(VERSION 3.9)
project(ratss)

set(CMAKE_MODULE_PATH
	${CMAKE_CURRENT_SOURCE_DIR}/cmake
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${MY_CXX_FLAGS}")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${MY_C_FLAGS}")


find_package(Threads)
find_package(LIBGMPXX REQUIRED)
find_package(LIBMPFR REQUIRED)
find_package(CGALSimple)
find_package(PkgConfig)

if (PKGCONFIG_FOUND)
	pkg_search_module(FPLLL fplll)
endif()

if (LIBCGAL_FOUND)
	set(LIBRATSS_CGAL_LINK_LIBRARIES
		CGALSimple::CGAL
	)
	set(LIBRATSS_COMPILE_DEFINITIONS
		${LIBRATSS_COMPILE_DEFINITIONS}
		"LIB_RATSS_WITH_CGAL=1"
	)
endif(LIBCGAL_FOUND)

if (FPLLL_FOUND)
	set(LIBRATSS_COMPILE_DEFINITIONS
		${LIBRATSS_COMPILE_DEFINITIONS}
		"LIB_RATSS_WITH_FPLLL=1"
	)
	set(LIBRATSS_COMPILE_OPTIONS
		${LIBRATSS_COMPILE_OPTIONS}
		${FPLLL_CFLAGS_OTHER}
	)
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
)

set(LIB_SOURCES_CPP
	src/Conversion.cpp
	src/ProjectSN.cpp
	src/ProjectS2.cpp
	src/Calc.cpp
	src/GeoCalc.cpp
	src/GeoCoord.cpp
	src/SphericalCoord.cpp
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

add_library(${PROJECT_NAME} STATIC
	${LIB_SOURCES_CPP}
)
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_14)
target_include_directories(${PROJECT_NAME} PUBLIC ${LIBRATSS_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} PUBLIC ${LIBRATSS_LINK_LIBRARIES})
target_compile_definitions(${PROJECT_NAME} PUBLIC ${LIBRATSS_COMPILE_DEFINITIONS})
target_compile_options(${PROJECT_NAME} PUBLIC ${LIBRATSS_COMPILE_OPTIONS})

add_subdirectory(common ${PROJECT_NAME}common EXCLUDE_FROM_ALL)
add_subdirectory(bench ${PROJECT_NAME}bench EXCLUDE_FROM_ALL)
add_subdirectory(tests ${PROJECT_NAME}tests EXCLUDE_FROM_ALL)
add_subdirectory(tools ${PROJECT_NAME}tools EXCLUDE_FROM_ALL)
