cmake_minimum_required(VERSION 3.9)
project(ratssbench)

include_directories(
	${PROJECT_GLOBAL_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_SOURCE_DIR}
)

MACRO(ADD_BENCH_TARGET _name _sourcefiles)
	add_executable("${PROJECT_NAME}_${_name}" "${_sourcefiles}")
	target_link_libraries("${PROJECT_NAME}_${_name}" ratss ratsscommon)
	add_dependencies("${PROJECT_NAME}_${_name}" ratss ratsscommon)
	set_target_properties("${PROJECT_NAME}_${_name}" PROPERTIES OUTPUT_NAME ${_name})
	set(RATSSBENCH_ALL_TARGETS ${RATSSBENCH_ALL_TARGETS} "${PROJECT_NAME}_${_name}")
ENDMACRO(ADD_BENCH_TARGET)

ADD_BENCH_TARGET(bitsize bitsize.cpp)
ADD_BENCH_TARGET(paper paper.cpp)
ADD_BENCH_TARGET(paper_table paper_table.cpp)

add_custom_target(${PROJECT_NAME}_all DEPENDS ${RATSSBENCH_ALL_TARGETS})
