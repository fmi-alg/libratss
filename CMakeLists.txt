cmake_minimum_required(VERSION 3.0)

set(CMAKE_MODULE_PATH
	${CMAKE_CURRENT_SOURCE_DIR}/cmake
)

project(ratss)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++0x")

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
	"${CMAKE_CURRENT_BINARY_DIR}/libratss.a"
)

set(LIBRATSS_LINK_LIBRARIES
	"${CMAKE_CURRENT_BINARY_DIR}/libratss.a"
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
		
	)
endif(CGAL_FOUND)

get_directory_property(hasParent PARENT_DIRECTORY)
#export correct link libs and include dirs
if(hasParent)
	set(LIBRATSS_INCLUDE_DIR
		"${CMAKE_CURRENT_SOURCE_DIR}/include"
		PARENT_SCOPE)
	
	set(LIBRATSS_LIBRARIES
		"${CMAKE_CURRENT_BINARY_DIR}/libratss.a"
		PARENT_SCOPE)
	
	set(LIBRATSS_LINK_LIBRARIES
		"${CMAKE_CURRENT_BINARY_DIR}/libratss.a"
		${CMAKE_THREAD_LIBS_INIT}
		${LIBGMPXX_LIBRARIES}
		${LIBMPFR_LIBRARIES}
		PARENT_SCOPE
	)
	if (CGAL_FOUND)
		set(LIBRATSS_LINK_LIBRARIES
			"CGAL"
			"CGAL_Core"
			"boost_thread"
			"boost_system"
			PARENT_SCOPE
		)
	endif(CGAL_FOUND)
endif()#hasParent

set(LIB_SOURCES_CPP
	src/Conversion.cpp
	src/ProjectS2.cpp
	src/Calc.cpp
	src/GeoCalc.cpp
	src/GeoCoord.cpp
	src/SphericalCoord.cpp
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

add_subdirectory(common ${PROJECT_NAME}common EXCLUDE_FROM_ALL)
add_subdirectory(bench ${PROJECT_NAME}bench EXCLUDE_FROM_ALL)
add_subdirectory(tests ${PROJECT_NAME}tests EXCLUDE_FROM_ALL)
