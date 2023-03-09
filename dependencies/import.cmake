
find_package(OpenGL REQUIRED)

macro(get_param_name name)
	string(FIND ${param} " " __param_name_sep)
	string(SUBSTRING ${param} 0 ${__param_name_sep} __param_name)
	set(${name} ${__param_name})
	unset(__param_name_sep)
	unset(__param_name)
endmacro()


function(add_subdirectory_params directory)
	foreach(param IN LISTS ARGN)
		get_param_name(param_name)
		set(${param_name}_value ${param_name})
		set(${param})
	endforeach()

	add_subdirectory(${directory})

	foreach(param IN LISTS ARGN)
		get_param_name(param_name)
		set(${param_name} ${${param_name}_value})
	endforeach()
endfunction()

add_subdirectory(${GOLXZNR_DEPENDENCIED_DIR}/glad/cmake glad_cmake)
glad_add_library(glad
	API ${GOLXZNR_GLAD_API}
	EXTENSIONS ${GOLXZNR_EXTENSIONS}
)
unset(dotless_og_ver)

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
