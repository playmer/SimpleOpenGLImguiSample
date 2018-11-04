cmake_minimum_required(VERSION 3.9.1 FATAL_ERROR) 
project(SimpleOpenGLImguiSample CXX)

set(AssetDirectory ${CMAKE_CURRENT_LIST_DIR}/Assets)

add_subdirectory(Dependencies)
add_subdirectory(Source)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT SimpleOpenGLImguiSample) 