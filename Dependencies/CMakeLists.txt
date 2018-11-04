# Set some options
option(GLFW_BUILD_EXAMPLES "Build the GLFW example programs" OFF)
option(GLFW_BUILD_TESTS "Build the GLFW test programs" OFF)
option(GLFW_BUILD_DOCS "Build the GLFW documentation" OFF)
option(GLFW_INSTALL "Generate installation target" OFF)
add_subdirectory(glfw)

option(BUILD_SHARED_LIBS     "Build shared instead of static libraries." OFF)
option(OPTION_BUILD_TOOLS    "Build tools."                              OFF)
option(OPTION_BUILD_EXAMPLES "Build examples."                           OFF)
add_subdirectory(glbinding)

add_subdirectory(imgui)
add_subdirectory(STB)