cmake_minimum_required(VERSION 3.9)
project(ratsscommon)

set(COMMON_LIB_SOURCES_CPP
	generators.cpp
	types.cpp
	stats.cpp
)

add_library(${PROJECT_NAME} STATIC ${COMMON_LIB_SOURCES_CPP})
target_link_libraries(${PROJECT_NAME} ratss)
