// dear imgui: standalone example application for GLFW + OpenGL 3, using programmable pipeline
// If you are new to dear imgui, see examples/README.txt and documentation at the top of imgui.cpp.
// (GLFW is a cross-platform general purpose library for handling windows, inputs, OpenGL/Vulkan graphics context creation, etc.)

#include <stdio.h>

#include "ImGuiSample.hpp"
#include "ApplicationContext.hpp"


int main(int, char**)
{
  SOIS::ApplicationInitialization();
  SOIS::ApplicationContext context;

  SOIS::ImGuiSample sample;

  // Main loop
  while (context.Update())
  {
    sample.Update();

    context.mClearColor = sample.mClearColor;
  }

  return 0;
}