cmake_minimum_required(VERSION 3.0)

IF(CMAKE_BUILD_TYPE MATCHES "^ltosanitize$")
	include(setup-lto)
	
	set(INLINE_IN_LTO_ENABLED
		True
		CACHE
		BOOL
		"Enable manual inline statements in lto builds"
		FORCE
	)
	
ENDIF()

IF ( (NOT LTO_SANITIZE_BUILD_TYPE_ADDED) )

	SET(CMAKE_CXX_FLAGS_LTOSANITIZE
		"-DNDEBUG -g -O3 -march=native -flto -fno-fat-lto-objects -fsanitize=address -fsanitize=undefined -fno-omit-frame-pointer"
		CACHE STRING "Flags used by the C++ compiler during lto builds."
		FORCE
	)
	SET(CMAKE_C_FLAGS_LTOSANITIZE
		"-DNDEBUG -g -O3 -march=native -flto -fno-fat-lto-objects -fsanitize=address -fsanitize=undefined -fno-omit-frame-pointer"
		CACHE STRING "Flags used by the C compiler during lto builds."
		FORCE
	)
	SET(CMAKE_EXE_LINKER_FLAGS_LTOSANITIZE
		""
		CACHE STRING "Flags used for linking binaries during ltosanitize builds."
		FORCE
	)
	SET(CMAKE_SHARED_LINKER_FLAGS_LTOSANITIZE
		""
		CACHE STRING "Flags used by the shared libraries linker during ltosanitize builds."
		FORCE
	)

	MARK_AS_ADVANCED(
		CMAKE_CXX_FLAGS_LTOSANITIZE
		CMAKE_C_FLAGS_LTOSANITIZE
		CMAKE_EXE_LINKER_FLAGS_LTOSANITIZE
		CMAKE_SHARED_LINKER_FLAGS_LTOSANITIZE
	)

	SET(CMAKE_CONFIGURATION_TYPES  
		"${CMAKE_CONFIGURATION_TYPES} ltosanitize"
	)

	SET(LTO_SANITIZE_BUILD_TYPE_ADDED
		"True"
		CACHE INTERNAL ""
		FORCE
	)
ENDIF()
