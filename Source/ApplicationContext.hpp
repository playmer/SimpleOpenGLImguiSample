#pragma once

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <glbinding/gl/gl.h>
#include <glbinding/glbinding.h>

#include <GLFW/glfw3.h> // Include glfw3.h after our OpenGL definitions


namespace SOIS
{
  // Call only once, loads OpenGL function pointers and other such work.
  void ApplicationInitialization();

  struct ApplicationContext
  {
  public:
    GLFWwindow* mWindow;
    ImVec4 mClearColor;
    bool mRunning;

    ApplicationContext();
    ~ApplicationContext();

    // Call when the application should update. Check the return
    // value to see if the application should continue.
    bool Update();

    // Call when you want the application to end.
    void EndApplication();

  private:
    void BeginFrame();
    void EndFrame();
  };
}