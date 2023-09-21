cmake_minimum_required(VERSION 3.23)

set(root ${CMAKE_CURRENT_SOURCE_DIR})
include(${root}/cmake/prelude.cmake)

project(golxzn_render
	VERSION 0.8.4
	DESCRIPTION "Render provider"
	HOMEPAGE_URL https://github.com/golxzn/render
	LANGUAGES C CXX
)
include(${root}/cmake/options.cmake)

load_sources_list(headers ${GOLXZNR_INCLUDE_DIR} ".h;.hpp")
load_sources_list(sources ${GOLXZNR_SOURCES_DIR} ".cpp")

add_library(${target} ${sources} ${headers})
add_library(golxzn::render ALIAS ${target})

include(GenerateExportHeader)
generate_export_header(${target}
	BASE_NAME ${target}
	EXPORT_FILE_NAME export/golxzn/graphics/export.hpp
	CUSTOM_CONTENT_FROM_VARIABLE pragma_suppress_c4251
)
if(NOT BUILD_SHARED_LIBS)
	target_compile_definitions(${target} PUBLIC GOLXZN_STATIC_DEFINE)
endif()

set_target_properties(${target}
	PROPERTIES
		CXX_STANDARD ${GOLXZNR_CXX_STANDARD}
		CXX_STANDARD_REQUIRED ON
		CXX_VISIBILITY_PRESET hidden
		VISIBILITY_INLINES_HIDDEN YES
		VERSION "${PROJECT_VERSION}"
		SOVERSION "${PROJECT_VERSION_MAJOR}"
		EXPORT_NAME ${GOLXZNR_PACKAGE_NAME}
		OUTPUT_NAME ${GOLXZNR_PACKAGE_NAME}
)


target_include_directories(${target} ${warning_guard}
	PUBLIC
		"$<BUILD_INTERFACE:${GOLXZNR_INCLUDE_DIR}>"
)
target_include_directories(${target} SYSTEM
	PUBLIC
		"$<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/export>"
)

include(${GOLXZNR_DEPENDENCIES_DIR}/import.cmake)
target_link_libraries(${target} PUBLIC ${dependencies})
target_include_directories(${target} PUBLIC ${include_directories})

include(${root}/cmake/install/rules.cmake)

if(GOLXZNR_BUILD_EXAMPLES)
	add_subdirectory(${GOLXZNR_EXAMPLES_DIR})
endif()
include(${root}/cmake/developer_mode.cmake)