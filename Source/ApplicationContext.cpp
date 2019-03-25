#include "imgui.h"
#include "imgui_impl_sdl.h"
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

glbinding::ProcAddress GLFunctionLoader(const char* aName)
{
  return reinterpret_cast<glbinding::ProcAddress>(SDL_GL_GetProcAddress(aName));
}

namespace SOIS
{
  void ApplicationInitialization()
  {
    // Setup SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0)
    {
      printf("Error: %s\n", SDL_GetError());
      exit(-1);
    }
  }

  ApplicationContext::ApplicationContext()
  {
        // Decide GL+GLSL versions
    #if __APPLE__
        // GL 3.2 Core + GLSL 150
        const char* glsl_version = "#version 150";
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG); // Always required on Mac
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    #else
        // GL 3.0 + GLSL 130
        const char* glsl_version = "#version 130";
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    #endif

    // Create window with graphics context
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_DisplayMode current;
    SDL_GetCurrentDisplayMode(0, &current);
    SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    mWindow = SDL_CreateWindow("Dear ImGui SDL2+OpenGL3 Sample Application", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, window_flags);
    mContext = SDL_GL_CreateContext(mWindow);
    SDL_GL_MakeCurrent(mWindow, mContext);
    SDL_GL_SetSwapInterval(1); // Enable vsync

    if (nullptr == mWindow)
    {
      return;
    }

    // Initialize OpenGL loader
    glbinding::initialize(GLFunctionLoader);

    gl::glEnable(gl::GL_DEBUG_OUTPUT);
    gl::glDebugMessageCallback(messageCallback, this);

    // Setup Dear ImGui binding
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGui_ImplSDL2_InitForOpenGL(mWindow, mContext);
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
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_DestroyWindow(mWindow);
    SDL_Quit();
  }


  void ApplicationContext::EndApplication()
  {
    mRunning = false;
  }

  bool ApplicationContext::Update()
  {
    EndFrame();

    if (false == mRunning)
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
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
      ImGui_ImplSDL2_ProcessEvent(&event);
      if (event.type == SDL_QUIT)
        mRunning = false;
      if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(mWindow))
        mRunning = false;
    }

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame(mWindow);
    ImGui::NewFrame();

    // Clear the viewport to prepare for user rendering.
    int display_w, display_h;
    SDL_GL_MakeCurrent(mWindow, mContext);
    ImGuiIO& io = ImGui::GetIO();
    gl::glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
    gl::glClearColor(mClearColor.x, mClearColor.y, mClearColor.z, mClearColor.w);
    gl::glClear(gl::GL_COLOR_BUFFER_BIT);
  }

  void ApplicationContext::EndFrame()
  {
    // Rendering Dear ImGui.
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    // Swap the buffers and prepare for next frame.
    SDL_GL_MakeCurrent(mWindow, mContext);
    SDL_GL_SwapWindow(mWindow);
  }
}