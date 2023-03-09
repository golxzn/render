
find_package(OpenGL REQUIRED)

add_subdirectory(${GOLXZNR_DEPENDENCIED_DIR}/glad/cmake glad_cmake)
glad_add_library(glad
	API ${GOLXZNR_GLAD_API}
	EXTENSIONS ${GOLXZNR_EXTENSIONS}
)

add_subdirectory_params(${GOLXZNR_DEPENDENCIED_DIR}/glfw
	"BUILD_SHARED_LIBS OFF"
	"GLFW_BUILD_DOCS OFF"
	"GLFW_INSTALL OFF"
)

add_subdirectory_params(${GOLXZNR_DEPENDENCIED_DIR}/core
	"GOLXZNC_DEVELOPER_MODE OFF"
)

add_subdirectory_params(${GOLXZNR_DEPENDENCIED_DIR}/glm
	"BUILD_TESTING OFF"
	"BUILD_STATIC_LIBS ON"
)
add_dependencies(${target} glm::glm glad)

list(APPEND dependencies
	OpenGL::GL
	glad
	glfw
	golxzn::core
)
