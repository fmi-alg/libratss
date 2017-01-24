cmake_minimum_required(VERSION 2.8)
project(ratssbench)

include_directories(
	${PROJECT_GLOBAL_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_SOURCE_DIR}
)

set(LINK_LIBS
	${ratsscommon_BINARY_DIR}/libratsscommon.a
	${LIBRATSS_LINK_LIBRARIES}
)

MACRO(ADD_BENCH_TARGET _name _sourcefiles)
	add_executable("${PROJECT_NAME}_${_name}" "${_sourcefiles}")
	target_link_libraries("${PROJECT_NAME}_${_name}" ${LINK_LIBS})
	add_dependencies("${PROJECT_NAME}_${_name}" ratss ratsscommon)
	set_target_properties("${PROJECT_NAME}_${_name}" PROPERTIES OUTPUT_NAME ${_name})
ENDMACRO(ADD_BENCH_TARGET)

ADD_BENCH_TARGET(bitsize bitsize.cpp)
ADD_BENCH_TARGET(paper paper.cpp)