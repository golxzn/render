if(NOT ${CMAKE_PROJECT_NAME}_DEVELOPER_MODE)
	return()
elseif(NOT PROJECT_IS_TOP_LEVEL)
	message(AUTHOR_WARNING "Developer mode is intended for developers of ${CMAKE_PROJECT_NAME}")
endif()

include(CTest)

if(BUILD_TESTING)
	add_subdirectory(${GOLXZNR_TESTS_DIR})
endif()

# set(autodir cmake/automatic)
# if(BUILD_MCSS_DOCS)
# 	include(${autodir}/docs.cmake)
# endif()
# if(ENABLE_COVERAGE)
# 	include(${autodir}/coverage.cmake)
# endif()

# include(${autodir}/lint-targets.cmake)

add_folders(Project)
