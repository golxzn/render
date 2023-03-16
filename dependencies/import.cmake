
find_package(OpenGL REQUIRED)

add_subdirectory(${GOLXZNR_DEPENDENCIES_DIR}/glad/cmake glad_cmake)
glad_add_library(glad
	API ${GOLXZNR_GLAD_API}
	EXTENSIONS ${GOLXZNR_EXTENSIONS}
)

add_subdirectory_params(${GOLXZNR_DEPENDENCIES_DIR}/glfw
	"BUILD_SHARED_LIBS OFF"
	"GLFW_BUILD_DOCS OFF"
	"GLFW_INSTALL OFF"
)

add_subdirectory_params(${GOLXZNR_DEPENDENCIES_DIR}/core
	"GOLXZNC_DEVELOPER_MODE OFF"
)

add_subdirectory(${GOLXZNR_DEPENDENCIES_DIR}/glm) # ${CMAKE_BINARY_DIR}/_deps/glm EXCLUDE_FROM_ALL)

add_subdirectory(${GOLXZNR_DEPENDENCIES_DIR}/stb_image)

add_dependencies(${target}
	glm::glm
	glad
)

list(APPEND dependencies
	OpenGL::GL
	glm::glm
	glad
	glfw
	stb::image
	golxzn::core
)
