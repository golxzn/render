include(CMakeFindDependencyMacro)
find_dependency(OpenGL)
find_dependency(Vulkan)

include("${CMAKE_CURRENT_LIST_DIR}/GolxznRenderTargets.cmake")
