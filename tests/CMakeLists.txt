cmake_minimum_required(VERSION 3.14)

project(CuteCoreTests LANGUAGES CXX)

include(../cmake/prelude.cmake)

# ---- Dependencies ----
if(PROJECT_IS_TOP_LEVEL)
	find_package(GolxznRender REQUIRED)
	enable_testing()
endif()

CPMAddPackage(
	NAME googletest
	GITHUB_REPOSITORY google/googletest
	GIT_TAG release-1.12.1
	VERSION 1.12.1
	OPTIONS "INSTALL_GTEST OFF" "gtest_force_shared_crt"
)

CPMAddPackage(
	NAME benchmark
	GITHUB_REPOSITORY google/benchmark
	VERSION 1.5.2
	OPTIONS "BENCHMARK_ENABLE_TESTING Off"
)

if(benchmark_ADDED)
	# enable c++11 to avoid compilation errors
	set_target_properties(benchmark PROPERTIES CXX_STANDARD 11)
endif()


# ---- Tests ----
load_sources_list(test_sources ${CMAKE_CURRENT_SOURCE_DIR}/sources ".h;.hpp;.cpp")
add_executable(CuteCoreTest ${test_sources})
target_link_libraries(CuteCoreTest PRIVATE
	golxzn::render
	GTest::gtest
	benchmark::benchmark
)
set_target_properties(CuteCoreTest PROPERTIES
	CXX_STANDARD 17
	CXX_STANDARD_REQUIRED ON
)

# ---- End-of-file commands ----
add_folders(Test)
