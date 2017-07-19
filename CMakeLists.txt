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

cmake_minimum_required(VERSION 3.0)
project(ratss)

set(CMAKE_MODULE_PATH
	${CMAKE_CURRENT_SOURCE_DIR}/cmake
)

set(MY_CXX_FLAGS "-std=gnu++0x")
set(MY_C_FLAGS "-std=c99")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${MY_CXX_FLAGS}")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${MY_C_FLAGS}")

include(add_buildtype_lto)

find_package(LIBGMPXX REQUIRED)
find_package(LIBMPFR REQUIRED)
find_package(Threads)

#optionally try to find cgal
find_package(CGAL COMPONENTS Core)

if (CGAL_FOUND)
	set(LIBRATSS_CGAL_LINK_LIBRARIES
		"CGAL"
		"CGAL_Core"
		"boost_thread"
		"boost_system"
	)
	set(LIBRATSS_CGAL_COMPILE_DEFINITIONS
		"LIB_RATSS_WITH_CGAL=1"
	)
endif(CGAL_FOUND)

set(LIBRATSS_INCLUDE_DIR
	"${CMAKE_CURRENT_SOURCE_DIR}/include"
	CACHE STRING "libratss include directories"
	FORCE
)

set(LIBRATSS_LIBRARIES
	${PROJECT_NAME}
	CACHE STRING "libratss libraries"
	FORCE
)

set(LIBRATSS_LINK_LIBRARIES
	${LIBRATSS_LIBRARIES}
	${LIBRATSS_CGAL_LINK_LIBRARIES}
	${CMAKE_THREAD_LIBS_INIT}
	${LIBGMPXX_LIBRARIES}
	${LIBMPFR_LIBRARIES}
	CACHE STRING "libratss link libraries"
	FORCE
)

set(LIBRATSS_COMPILE_DEFINITIONS
	${LIBRATSS_CGAL_COMPILE_DEFINITIONS}
	CACHE STRING "libratss compile definitions"
	FORCE
)

mark_as_advanced(
	LIBRATSS_INCLUDE_DIR
	LIBRATSS_LIBRARIES
	LIBRATSS_LINK_LIBRARIES
	LIBRATSS_COMPILE_DEFINITIONS
)

include_directories(
	${LIBRATSS_INCLUDE_DIR}
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
	)
endif(CGAL_FOUND)

add_library(${PROJECT_NAME} STATIC
	${LIB_SOURCES_CPP}
)

if (CGAL_FOUND)
	target_compile_definitions(${PROJECT_NAME} PUBLIC ${LIBRATSS_COMPILE_DEFINITIONS})
endif(CGAL_FOUND)

add_subdirectory(common ${PROJECT_NAME}common EXCLUDE_FROM_ALL)
add_subdirectory(bench ${PROJECT_NAME}bench EXCLUDE_FROM_ALL)
add_subdirectory(tests ${PROJECT_NAME}tests EXCLUDE_FROM_ALL)
add_subdirectory(tools ${PROJECT_NAME}tools EXCLUDE_FROM_ALL)
