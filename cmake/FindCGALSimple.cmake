#  LIBCGAL_INCLUDE_DIR - Where to find cgal include sub-directory.
#  LIBCGAL_LIBRARIES   - List of libraries when using LIBCGAL.
#  LIBCGAL_FOUND       - True if LIBCGAL found.
IF (LIBCGAL_INCLUDE_DIR)
  # Already in cache, be silent.
  SET(LIBCGAL_FIND_QUIETLY TRUE)
ENDIF (LIBCGAL_INCLUDE_DIR)

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

	message(STATUS "CGAL include dir: ${LIBCGAL_INCLUDE_DIR}")

	SET(LIBCGAL_NAMES CGAL CGAL_Core )
	FIND_LIBRARY(LIBCGAL_LIBRARY NAMES ${LIBCGAL_NAMES} )

	# Handle the QUIETLY and REQUIRED arguments and set LIBCGAL_FOUND to
	# TRUE if all listed variables are TRUE.
	INCLUDE(FindPackageHandleStandardArgs)
	FIND_PACKAGE_HANDLE_STANDARD_ARGS(
		LIBCGAL DEFAULT_MSG
		LIBCGAL_LIBRARY LIBCGAL_INCLUDE_DIR
	)

	IF(LIBCGAL_FOUND)
		SET(LIBCGAL_LIBRARIES
			${LIBCGAL_LIBRARY}
			${Boost_THREAD_LIBRARY}
			${Boost_SYSTEM_LIBRARY}
			${LIBMPFR_LIBRARIES}
			${LIBGMPXX_LIBRARIES}
		)
	ELSE(LIBCGAL_FOUND)
		SET(LIBCGAL_LIBRARIES )
	ENDIF(LIBCGAL_FOUND)

	MARK_AS_ADVANCED( LIBCGAL_LIBRARY LIBCGAL_INCLUDE_DIR )
ENDIF()
