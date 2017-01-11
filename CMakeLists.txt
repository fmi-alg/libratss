# This is the build file to build libratss
# It can be build as a stand-alone project as well as a sub-project
# If this is intended as a sub-project then make sure to add the appropriate compile definitions to your project
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
# target_compile_definitions(${PROJECT_NAME} PUBLIC ${LIBRATSS_COMPILE_DEFINITIONS})
# add_dependencies(${PROJECT_NAME} ratss)

cmake_minimum_required(VERSION 3.0)

set(CMAKE_MODULE_PATH
	${CMAKE_CURRENT_SOURCE_DIR}/cmake
)

project(ratss)

set(MY_CXX_FLAGS "-std=gnu++0x")
set(MY_C_FLAGS "-std=c99")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${MY_CXX_FLAGS}")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${MY_C_FLAGS}")

set(MY_LTO_CXX_FLAGS "${MY_CXX_FLAGS} -DNDEBUG -O3 -march=native -flto -ffat-lto-objects -frounding-math")
set(MY_LTO_C_FLAGS "${MY_C_FLAGS} -DNDEBUG -O3 -march=native -flto -ffat-lto-objects -frounding-math")
set(MY_LTO_LINKER_FLAGS "")

SET(CMAKE_CXX_FLAGS_LTO
	"${MY_LTO_CXX_FLAGS}"
	CACHE STRING "Flags used by the C++ compiler during lto builds."
	FORCE
)
SET(CMAKE_C_FLAGS_LTO
	"${MY_PROFILE_GENERATE_C_FLAGS}"
	CACHE STRING "Flags used by the C compiler during lto builds."
	FORCE
)
SET(CMAKE_EXE_LINKER_FLAGS_LTO
	"${MY_PROFILE_GENERATE_LINKER_FLAGS}"
	CACHE STRING "Flags used for linking binaries during lto builds."
	FORCE
)
SET(CMAKE_SHARED_LINKER_FLAGS_LTO
	""
	CACHE STRING "Flags used by the shared libraries linker during lto builds."
	FORCE
)
MARK_AS_ADVANCED(
	CMAKE_CXX_FLAGS_LTO
	CMAKE_C_FLAGS_LTO
	CMAKE_EXE_LINKER_FLAGS_LTO
	CMAKE_SHARED_LINKER_FLAGS_LTO
)

find_package(LIBGMPXX REQUIRED)
find_package(LIBMPFR REQUIRED)
find_package(Threads)

#optionally try to find cgal
find_package(CGAL COMPONENTS Core)

include_directories(
# 	${CMAKE_SOURCE_DIR}
# 	${CMAKE_CURRENT_SOURCE_DIR}
# 	${CMAKE_SOURCE_DIR}/include
	${CMAKE_CURRENT_SOURCE_DIR}/include
)

set(LIBRATSS_INCLUDE_DIR
	"${CMAKE_CURRENT_SOURCE_DIR}/include"
)

set(LIBRATSS_LIBRARIES
	ratss
)

set(LIBRATSS_LINK_LIBRARIES
	ratss
	${CMAKE_THREAD_LIBS_INIT}
	${LIBGMPXX_LIBRARIES}
	${LIBMPFR_LIBRARIES}
)

if (CGAL_FOUND)
	set(LIBRATSS_LINK_LIBRARIES
		${LIBRATSS_LINK_LIBRARIES}
		"CGAL"
		"CGAL_Core"
		"boost_thread"
		"boost_system"
		${LIBRATSS_LINK_LIBRARIES}
	)
	set(LIBRATSS_COMPILE_DEFINITIONS
		"LIB_RATSS_WITH_CGAL=1"
	)
endif(CGAL_FOUND)

get_directory_property(hasParent PARENT_DIRECTORY)
#export correct link libs and include dirs
if(hasParent)
	set(LIBRATSS_INCLUDE_DIR
		${LIBRATSS_INCLUDE_DIR}
		PARENT_SCOPE)
	
	set(LIBRATSS_LIBRARIES
		${LIBRATSS_LIBRARIES}
		PARENT_SCOPE)
	
	set(LIBRATSS_LINK_LIBRARIES
		${LIBRATSS_LINK_LIBRARIES}
		PARENT_SCOPE
	)
	
	set(LIBRATSS_COMPILE_DEFINITIONS
		${LIBRATSS_COMPILE_DEFINITIONS}
		PARENT_SCOPE
	)
endif()#hasParent

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
