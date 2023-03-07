
set(target ${CMAKE_PROJECT_NAME})
set(dependencies "")

set(GOLXZNR_PACKAGE_NAME GolxznRender)
set(GOLXZNR_INCLUDE_DIR code/include)
set(GOLXZNR_SOURCES_DIR code/src)
set(GOLXZNR_TESTS_DIR tests)
set(GOLXZNR_DEPENDENCIED_DIR dependencies)
set(GOLXZNR_CXX_STANDARD 17)

option(GOLXZNR_DEVELOPER_MODE "Enable developer mode" OFF)
if(GOLXZNR_DEVELOPER_MODE)
	option(BUILD_MCSS_DOCS "Build documentation using Doxygen and m.css" OFF)
	option(ENABLE_COVERAGE "Enable coverage support separate from CTest's" OFF)
endif()

message(STATUS "-- -- -- -- -- -- -- -- -- -- -- -- Variables -- -- -- -- -- -- -- -- -- -- -- -- --")
message(STATUS "Target:                         | ${target}")
message(STATUS "Package:                        | ${GOLXZNR_PACKAGE_NAME}")
message(STATUS "Root:                           | ${root}")
message(STATUS "C++ standard:                   | ${GOLXZNR_CXX_STANDARD}")
message(STATUS "Is top level:                   | ${PROJECT_IS_TOP_LEVEL}")
message(STATUS "Include directory:              | ${GOLXZNR_INCLUDE_DIR}")
message(STATUS "Sources directory:              | ${GOLXZNR_SOURCES_DIR}")
message(STATUS "Tests directory:                | ${GOLXZNR_TESTS_DIR}")
message(STATUS "Dependencies directory:         | ${GOLXZNR_CXX_STANDARD}")
message(STATUS "-- -- -- -- -- -- -- -- -- -- -- -- -- --- -- -- -- -- -- -- -- -- -- -- -- -- -- --")

