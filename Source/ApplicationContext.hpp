#pragma once

#include "imgui.h"

#include <SDL.h> // Include glfw3.h after our OpenGL definitions

#include <glbinding/gl/gl.h>
#include <glbinding/glbinding.h>

#include "glm/glm.hpp"

namespace SOIS
{
  // Call only once, loads OpenGL function pointers and other such work.
  void ApplicationInitialization();

  struct ApplicationContext
  {
  public:
    SDL_GLContext mContext;
    SDL_Window* mWindow;
    glm::vec4 mClearColor;
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