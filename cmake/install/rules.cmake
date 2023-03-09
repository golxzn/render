if(CMAKE_SKIP_INSTALL_RULES)
	return()
endif()

if(PROJECT_IS_TOP_LEVEL)
	set(CMAKE_INSTALL_INCLUDEDIR include/ CACHE PATH "")
endif()

include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

# find_package(<package>) call for consumers to find this project
set(package ${GOLXZNR_PACKAGE_NAME})

install(DIRECTORY ${${CMAKE_PROJECT_NAME}_include_dir}/ "${PROJECT_BINARY_DIR}/export/"
	DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
	COMPONENT ${GOLXZN_PACKAGE_NAME}_Development
)

install(TARGETS ${target} EXPORT ${GOLXZN_PACKAGE_NAME}Targets
	RUNTIME COMPONENT ${GOLXZN_PACKAGE_NAME}_Runtime
	LIBRARY COMPONENT ${GOLXZN_PACKAGE_NAME}_Runtime
	NAMELINK_COMPONENT ${GOLXZN_PACKAGE_NAME}_Development
	ARCHIVE COMPONENT ${GOLXZN_PACKAGE_NAME}_Development
	INCLUDES DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
)

write_basic_package_version_file("${package}ConfigVersion.cmake" COMPATIBILITY SameMajorVersion)

# Allow package maintainers to freely override the path for the configs
set(${GOLXZN_PACKAGE_NAME}_INSTALL_CMAKEDIR "${CMAKE_INSTALL_LIBDIR}/cmake/${package}"
	CACHE PATH "CMake package config location relative to the install prefix"
)
mark_as_advanced(${GOLXZN_PACKAGE_NAME}_INSTALL_CMAKEDIR)

install(FILES cmake/install/config.cmake
	DESTINATION "${${GOLXZN_PACKAGE_NAME}_INSTALL_CMAKEDIR}"
	RENAME "${package}Config.cmake"
	COMPONENT ${GOLXZN_PACKAGE_NAME}_Development
)

install(FILES "${PROJECT_BINARY_DIR}/${package}ConfigVersion.cmake"
	DESTINATION "${${GOLXZN_PACKAGE_NAME}_INSTALL_CMAKEDIR}"
	COMPONENT ${GOLXZN_PACKAGE_NAME}_Development
)

install(EXPORT ${GOLXZN_PACKAGE_NAME}Targets
	NAMESPACE ${GOLXZN_PACKAGE_NAME}::
	DESTINATION "${${GOLXZN_PACKAGE_NAME}_INSTALL_CMAKEDIR}"
	COMPONENT ${GOLXZN_PACKAGE_NAME}_Development
)

if(PROJECT_IS_TOP_LEVEL)
	include(CPack)
endif()
