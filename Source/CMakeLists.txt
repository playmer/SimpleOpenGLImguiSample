add_executable(SimpleOpenGLImguiSample "")

target_sources(SimpleOpenGLImguiSample 
PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/main.cpp
    ${CMAKE_CURRENT_LIST_DIR}/ImGuiSample.hpp
    ${CMAKE_CURRENT_LIST_DIR}/ApplicationContext.hpp
    ${CMAKE_CURRENT_LIST_DIR}/ApplicationContext.cpp
)

target_include_directories(SimpleOpenGLImguiSample 
PRIVATE 
    ${CMAKE_CURRENT_LIST_DIR}
    ${DependenciesDirectory}/glm)

target_link_libraries(SimpleOpenGLImguiSample 
PRIVATE 
    glbinding
    SDL2-static
    SDL2main
    glm_static
    imgui
    STB
)

target_compile_definitions(SimpleOpenGLImguiSample PRIVATE GLFW_INCLUDE_NONE)

if (${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
    target_link_libraries(SimpleOpenGLImguiSample PRIVATE opengl32.lib)

    target_compile_options(SimpleOpenGLImguiSample PRIVATE 
    PUBLIC
        -permissive- -std:c++17
    PRIVATE
        -WX- -W4
    )
endif()

set_target_properties(SimpleOpenGLImguiSample PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${AssetDirectory}/Bin)
