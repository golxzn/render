# ---- In-source guard ----

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)
	message(FATAL_ERROR
		"In-source builds are not supported. "
		"Please read the BUILDING document before trying to build this project. "
		"You may need to delete 'CMakeCache.txt' and 'CMakeFiles/' first."
	)
endif()

# This variable is set by project() in CMake 3.21+
if(${CMAKE_VERSION} VERSION_LESS "3.21.0")
	string(COMPARE EQUAL "${CMAKE_SOURCE_DIR}" "${PROJECT_SOURCE_DIR}" PROJECT_IS_TOP_LEVEL)
endif()

if (EXISTS ${root}/dependencies/core/cmake/tools.cmake)
	include(${root}/dependencies/core/cmake/tools.cmake)
else()
	include(${root}/cmake/tools.cmake)
endif()
