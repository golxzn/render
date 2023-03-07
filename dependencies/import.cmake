
find_package(OpenGL REQUIRED)

set(GOLXZNC_DEVELOPER_MODE OFF)
add_subdirectory(${GOLXZNR_DEPENDENCIED_DIR}/core)

list(APPEND dependencies
	OpenGL::GL
	golxzn::core
)
