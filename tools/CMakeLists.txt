cmake_minimum_required(VERSION 3.9)
project(ratsstools)

include_directories(
	${PROJECT_GLOBAL_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_SOURCE_DIR}
)

set(TOOLS_LIB_SOURCES_CPP
	types.cpp
)

add_library(${PROJECT_NAME} STATIC ${TOOLS_LIB_SOURCES_CPP})
target_link_libraries(${PROJECT_NAME} ratsscommon ratss)

MACRO(ADD_TOOLS_TARGET _name _sourcefiles)
	add_executable("${PROJECT_NAME}_${_name}" "${_sourcefiles}")
	target_link_libraries("${PROJECT_NAME}_${_name}" ratsstools)
	set_target_properties("${PROJECT_NAME}_${_name}" PROPERTIES OUTPUT_NAME ${_name})
	set(RATSSTOOLS_ALL_TARGETS ${RATSSTOOLS_ALL_TARGETS} "${PROJECT_NAME}_${_name}")
ENDMACRO(ADD_TOOLS_TARGET)

ADD_TOOLS_TARGET(proj proj.cpp)
ADD_TOOLS_TARGET(rndpoints rndpoints.cpp)
ADD_TOOLS_TARGET(snap_poles snap_poles.cpp)

if (FPLLL_FOUND)
	ADD_TOOLS_TARGET(ratapx ratapx.cpp)
endif(FPLLL_FOUND)

add_custom_target(${PROJECT_NAME}_all DEPENDS ${RATSSTOOLS_ALL_TARGETS})
