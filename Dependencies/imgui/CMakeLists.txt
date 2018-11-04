get_filename_component(ParentDirectory ${CMAKE_CURRENT_SOURCE_DIR} DIRECTORY)
include_directories(${ParentDirectory} ${CMAKE_CURRENT_SOURCE_DIR})

add_library(imgui 
    ImGuizmo.cpp
    ImGuizmo.h
    ImSequencer.cpp
    ImSequencer.h
    imconfig.h
    imgui.cpp
    imgui.h
    imgui_demo.cpp
    imgui_draw.cpp
    imgui_widgets.cpp
    imgui_internal.h
    imgui_stl.cpp
    imgui_stl.h
    imgui_impl_glfw.cpp
    imgui_impl_glfw.h
    imgui_impl_opengl3.cpp
    imgui_impl_opengl3.h)

target_include_directories(imgui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(imgui PUBLIC STB glfw glbinding)
