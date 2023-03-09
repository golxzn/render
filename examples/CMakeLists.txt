cmake_minimum_required(VERSION 3.23)

project(golxzn_examples LANGUAGES CXX)

# @todo: Normalize this shit. Looks really invalid

add_executable(golxzn_examples ${CMAKE_CURRENT_SOURCE_DIR}/opengl_triangle/main.cpp)
set_target_properties(golxzn_examples PROPERTIES
	CXX_STANDARD ${GOLXZNR_CXX_STANDARD}
)
target_link_libraries(golxzn_examples PUBLIC
	golxzn::core
	golxzn::render
)
