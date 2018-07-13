#  LIBCGAL_INCLUDE_DIR - Where to find cgal include sub-directory.
#  LIBCGAL_LIBRARIES   - List of libraries when using LIBCGAL.
#  LIBCGAL_FOUND       - True if LIBCGAL found.
if (LIBCGAL_INCLUDE_DIR)
  # Already in cache, be silent.
  set(LIBCGAL_FIND_QUIETLY TRUE)
endif (LIBCGAL_INCLUDE_DIR)

find_package(CGAL COMPONENTS Core)
find_package(Boost COMPONENTS thread system)
find_package(LIBMPFR)
find_package(LIBGMPXX)

if( ${Boost_THREAD_FOUND} AND ${Boost_SYSTEM_FOUND} AND ${CGAL_FOUND})

	find_path(LIBCGAL_INCLUDE_DIR
			NAMES CGAL/Exact_predicates_exact_constructions_kernel.h
			HINTS ENV CGAL_INC_DIR
			PATH_SUFFIXES include
			DOC "The directory containing the CGAL header files"
			)

	set(LIBCGAL_NAMES CGAL)
	set(LIBCGAL_CORE_NAMES CGAL_Core )
	find_library(LIBCGAL_LIBRARY NAMES ${LIBCGAL_NAMES} )
	find_library(LIBCGAL_CORE_LIBRARY NAMES ${LIBCGAL_CORE_NAMES} )

	# Handle the QUIETLY and REQUIRED arguments and set LIBCGAL_FOUND to
	# TRUE if all listed variables are TRUE.
	include(FindPackageHandleStandardArgs)
	FIND_PACKAGE_HANDLE_STANDARD_ARGS(
		LIBCGAL DEFAULT_MSG
		LIBCGAL_LIBRARY LIBCGAL_INCLUDE_DIR
	)
	
	FIND_PACKAGE_HANDLE_STANDARD_ARGS(
		LIBCGAL_CORE DEFAULT_MSG
		LIBCGAL_CORE_LIBRARY LIBCGAL_INCLUDE_DIR
	)

	if(LIBCGAL_FOUND AND LIBCGAL_CORE_FOUND)
		set(LIBCGAL_LIBRARIES
			${LIBCGAL_LIBRARY}
			${LIBCGAL_CORE_LIBRARY}
			${Boost_THREAD_LIBRARY}
			${Boost_SYSTEM_LIBRARY}
			${LIBMPFR_LIBRARIES}
			${LIBGMPXX_LIBRARIES}
		)
		set(LIBCGAL_INCLUDE_DIR 
			${LIBCGAL_INCLUDE_DIR}
			${BOOST_INCLUDE_DIR}
			${LIBMPFR_INCLUDE_DIR}
			${LIBGMPXX_INCLUDE_DIR}
		)
		set(LIBCGAL_COMPILE_OPTIONS
			"-frounding-math"
			CACHE STRING "cgal compile definitions"
			FORCE
		)
		
		message(STATUS "CGAL include dirs: ${LIBCGAL_INCLUDE_DIR}")
		message(STATUS "CGAL libraries: ${LIBCGAL_LIBRARIES}")
		message(STATUS "CGAL may need specific compiler options like -frounding-math which have to be added by the caller if you are not using imported targets")
		
		if (NOT TARGET CGALSimple::CGAL)
			add_library(CGALSimple::CGAL INTERFACE IMPORTED)
			set_target_properties(CGALSimple::CGAL PROPERTIES
				INTERFACE_INCLUDE_DIRECTORIES "${LIBCGAL_INCLUDE_DIR}"
			)
			set_target_properties(CGALSimple::CGAL PROPERTIES
				INTERFACE_COMPILE_OPTIONS "${LIBCGAL_COMPILE_OPTIONS}"
			)
			set_target_properties(CGALSimple::CGAL PROPERTIES
				INTERFACE_LINK_LIBRARIES "${LIBCGAL_LIBRARIES}"
			)
		endif()
	else()
		set(LIBCGAL_LIBRARIES )
	endif()

	mark_as_advanced( LIBCGAL_LIBRARY LIBCGAL_INCLUDE_DIR )
endif()
