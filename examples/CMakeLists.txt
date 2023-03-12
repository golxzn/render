cmake_minimum_required(VERSION 3.23)

project(golxzn_examples LANGUAGES CXX)

# @todo: Normalize this shit. Looks really invalid

set(example_dir ${GOLXZNR_EXAMPLES_DIR}/${GOLXZNR_EXAMPLE_TARGET})

load_sources_list(example_headers ${example_dir} ".h;.hpp")
load_sources_list(example_sources ${example_dir} ".cpp")

add_executable(golxzn_examples ${example_sources} ${example_headers})
set_target_properties(golxzn_examples PROPERTIES
	CXX_STANDARD ${GOLXZNR_CXX_STANDARD}
)
target_link_libraries(golxzn_examples PUBLIC
	golxzn::core
	golxzn::render
)
