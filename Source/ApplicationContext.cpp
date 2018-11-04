#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "ApplicationContext.hpp"

static char const* Source(gl::GLenum source)
{
  switch (source)
  {
    case gl::GL_DEBUG_SOURCE_API: return "DEBUG_SOURCE_API";
    case gl::GL_DEBUG_SOURCE_WINDOW_SYSTEM: return "DEBUG_SOURCE_WINDOW_SYSTEM";
    case gl::GL_DEBUG_SOURCE_SHADER_COMPILER: return "DEBUG_SOURCE_SHADER_COMPILER";
    case gl::GL_DEBUG_SOURCE_THIRD_PARTY: return "DEBUG_SOURCE_THIRD_PARTY";
    case gl::GL_DEBUG_SOURCE_APPLICATION: return "DEBUG_SOURCE_APPLICATION";
    case gl::GL_DEBUG_SOURCE_OTHER: return "DEBUG_SOURCE_OTHER";
    default: return "unknown";
  }
}

static char const* Severity(gl::GLenum severity)
{
  switch (severity)
  {
    case gl::GL_DEBUG_SEVERITY_HIGH: return "DEBUG_SEVERITY_HIGH";
    case gl::GL_DEBUG_SEVERITY_MEDIUM: return "DEBUG_SEVERITY_MEDIUM";
    case gl::GL_DEBUG_SEVERITY_LOW: return "DEBUG_SEVERITY_LOW";
    case gl::GL_DEBUG_SEVERITY_NOTIFICATION: return "DEBUG_SEVERITY_NOTIFICATION";
    default: return "unknown";
  }
}


static char const* Type(gl::GLenum type)
{
  switch (type)
  {
    case gl::GL_DEBUG_TYPE_ERROR: return "DEBUG_TYPE_ERROR";
    case gl::GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: return "DEBUG_TYPE_DEPRECATED_BEHAVIOR";
    case gl::GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: return "DEBUG_TYPE_UNDEFINED_BEHAVIOR";
    case gl::GL_DEBUG_TYPE_PORTABILITY: return "DEBUG_TYPE_PORTABILITY";
    case gl::GL_DEBUG_TYPE_PERFORMANCE: return "DEBUG_TYPE_PERFORMANCE";
    case gl::GL_DEBUG_TYPE_MARKER: return "DEBUG_TYPE_MARKER";
    case gl::GL_DEBUG_TYPE_PUSH_GROUP: return "DEBUG_TYPE_PUSH_GROUP";
    case gl::GL_DEBUG_TYPE_POP_GROUP: return "DEBUG_TYPE_POP_GROUP";
    case gl::GL_DEBUG_TYPE_OTHER: return "DEBUG_TYPE_OTHER";
    default: return "unknown";
  }
}

static 
void GL_APIENTRY messageCallback(gl::GLenum source,
                                 gl::GLenum type,
                                 gl::GLuint id,
                                 gl::GLenum severity,
                                 gl::GLsizei length,
                                 const gl::GLchar* message,
                                 const void* userParam)
{
  if (gl::GL_DEBUG_SEVERITY_NOTIFICATION == severity)
  {
    return;
  }

  printf("GL DEBUG CALLBACK:\n    Source = %s\n    type = %s\n    severity = %s\n    message = %s\n",
         Source(source),
         Type(type),
         Severity(severity),
         message);
}


static void glfw_error_callback(int aError, const char* aDescription)
{
  fprintf(stderr, "Glfw Error %d: %s\n", aError, aDescription);
}

namespace SOIS
{
  void ApplicationInitialization()
  {
    // Setup window
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
    {
      return;
    }
  }

  ApplicationContext::ApplicationContext()
  {
    // Decide GL+GLSL versions
    #if __APPLE__
      // GL 3.2 + GLSL 150
      const char* glsl_version = "#version 150";
      glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
      glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
      glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
      glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
    #else
      // GL 3.0 + GLSL 130
      const char* glsl_version = "#version 130";
      glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
      glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
      //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
      //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
    #endif

    // Create window with graphics context
    mWindow = glfwCreateWindow(1280, 720, "Dear ImGui GLFW+OpenGL3 Sample Application", NULL, NULL);

    if (nullptr == mWindow)
    {
      return;
    }

    glfwMakeContextCurrent(mWindow);
    glfwSwapInterval(1); // Enable vsync

    // Initialize OpenGL loader
    glbinding::initialize(glfwGetProcAddress);

    gl::glEnable(gl::GL_DEBUG_OUTPUT);
    gl::glDebugMessageCallback(messageCallback, this);

    // Setup Dear ImGui binding
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGui_ImplGlfw_InitForOpenGL(mWindow, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Setup style
    ImGui::StyleColorsDark();


    // We run a begin frame here once, because the update function (meant to 
    // be run as the condition to a while loop.) needs to run both the begin
    // frame work (which it does after checking if it should continue the 
    // frame), and the end frame work (which it does before checking if it
    // should continue running). So we need to prepare for the first end frame.
    BeginFrame();
  }

  ApplicationContext::~ApplicationContext()
  {
    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(mWindow);
    glfwTerminate();
  }


  void ApplicationContext::EndApplication()
  {
    mRunning = false;
  }

  bool ApplicationContext::Update()
  {
    EndFrame();

    if (false == mRunning ||
        false != glfwWindowShouldClose(mWindow))
    {
      return false;
    }

    BeginFrame();

    return true;
  }

  void ApplicationContext::BeginFrame()
  {
    // Poll and handle events (inputs, window resize, etc.)
    // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
    // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
    // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
    // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
    glfwPollEvents();

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // Clear the viewport to prepare for user rendering.
    int display_w, display_h;
    glfwMakeContextCurrent(mWindow);
    glfwGetFramebufferSize(mWindow, &display_w, &display_h);
    gl::glViewport(0, 0, display_w, display_h);
    gl::glClearColor(mClearColor.x, mClearColor.y, mClearColor.z, mClearColor.w);
    gl::glClear(gl::GL_COLOR_BUFFER_BIT);
  }

  void ApplicationContext::EndFrame()
  {
    // Rendering Dear ImGui.
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    // Swap the buffers and prepare for next frame.
    glfwMakeContextCurrent(mWindow);
    glfwSwapBuffers(mWindow);
  }
}