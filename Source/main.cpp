///////////////////////////////////////////////////////////////////////////
// This is a sample application based on the Hello-Triangle example from 
// LearnOpenGL.com and the ImGui demo application. It implements the ImGui
// demo in the ImGuiSample struct and the Hello-Triangle example here in 
// this file. It is intended to be an easy way to get up and running in
// graphics programming without worrying about some of the other set up
// (cmake, dependency management, platform management, GL loading layers,
// etc...)
// 
// Most of your set up can happen after the context creation, and any
// updates can happen inside of the while loop.
//
// Dependencies:
// glbinding
// GLFW
// GLM
// ImGui
// ImGuizmo
// stb libraries
///////////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <iostream>

#include "ImGuiSample.hpp"
#include "ApplicationContext.hpp"

int main(int, char**)
{
  ///////////////////////////////////////////////////////////////////////////
  // Application set up, after this you can run "any" OpenGL or ImGui code
  // you'd like in the while loop down below.
  ///////////////////////////////////////////////////////////////////////////
  SOIS::ApplicationInitialization();
  SOIS::ApplicationContext context;

  ///////////////////////////////////////////////////////////////////////////
  // This is not required to call any ImGui code you write. It's simply
  // here to show you what's possible by showing you the ImGui base demo.
  // Feel free to remove it and replace with your own ImGui code. (Or no
  // ImGui code if you'd like.)
  ///////////////////////////////////////////////////////////////////////////
  SOIS::ImGuiSample sample;


  // The following is the Hello-Triangle example from learnopengl.com
  const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";
  const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);\n"
    "}\n\0";

  // build and compile our shader program
  // ------------------------------------
  // vertex shader
  int vertexShader = gl::glCreateShader(gl::GL_VERTEX_SHADER);
  gl::glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
  gl::glCompileShader(vertexShader);
  // check for shader compile errors
  int success;
  char infoLog[512];
  gl::glGetShaderiv(vertexShader, gl::GL_COMPILE_STATUS, &success);
  if (!success)
  {
    gl::glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
  }
  // fragment shader
  int fragmentShader = gl::glCreateShader(gl::GL_FRAGMENT_SHADER);
  gl::glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
  gl::glCompileShader(fragmentShader);
  // check for shader compile errors
  gl::glGetShaderiv(fragmentShader, gl::GL_COMPILE_STATUS, &success);
  if (!success)
  {
    gl::glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
    std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
  }
  // link shaders
  int shaderProgram = gl::glCreateProgram();
  gl::glAttachShader(shaderProgram, vertexShader);
  gl::glAttachShader(shaderProgram, fragmentShader);
  gl::glLinkProgram(shaderProgram);
  // check for linking errors
  gl::glGetProgramiv(shaderProgram, gl::GL_LINK_STATUS, &success);
  if (!success)
  {
    gl::glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
    std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
  }
  gl::glDeleteShader(vertexShader);
  gl::glDeleteShader(fragmentShader);

  // set up vertex data (and buffer(s)) and configure vertex attributes
  // ------------------------------------------------------------------
  float vertices[] = {
       0.5f,  0.5f, 0.0f,  // top right
       0.5f, -0.5f, 0.0f,  // bottom right
      -0.5f, -0.5f, 0.0f,  // bottom left
      -0.5f,  0.5f, 0.0f   // top left 
  };
  unsigned int indices[] = {  // note that we start from 0!
      0, 1, 3,  // first Triangle
      1, 2, 3   // second Triangle
  };
  unsigned int VBO, VAO, EBO;
  gl::glGenVertexArrays(1, &VAO);
  gl::glGenBuffers(1, &VBO);
  gl::glGenBuffers(1, &EBO);
  // bind the Vertex Array Object first, then bind and set vertex buffer(s), 
  // and then configure vertex attributes(s).
  gl::glBindVertexArray(VAO);

  gl::glBindBuffer(gl::GL_ARRAY_BUFFER, VBO);
  gl::glBufferData(gl::GL_ARRAY_BUFFER, sizeof(vertices), vertices, gl::GL_STATIC_DRAW);

  gl::glBindBuffer(gl::GL_ELEMENT_ARRAY_BUFFER, EBO);
  gl::glBufferData(gl::GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, gl::GL_STATIC_DRAW);

  gl::glVertexAttribPointer(0, 3, gl::GL_FLOAT, gl::GL_FALSE, 3 * sizeof(float), (void*)0);
  gl::glEnableVertexAttribArray(0);

  // note that this is allowed, the call to glVertexAttribPointer registered
  // VBO as the vertex attribute's bound vertex buffer object so afterwards 
  // we can safely unbind
  gl::glBindBuffer(gl::GL_ARRAY_BUFFER, 0);

  // remember: do NOT unbind the EBO while a VAO is active as the bound 
  // element buffer object IS stored in the VAO; keep the EBO bound.
  //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  // You can unbind the VAO afterwards so other VAO calls won't accidentally
  // modify this VAO, but this rarely happens. Modifying other VAOs requires
  // a call to glBindVertexArray anyways so we generally don't unbind VAOs
  // (nor VBOs) when it's not directly necessary.
  gl::glBindVertexArray(0);

  // Main loop
  while (context.Update())
  {
    ///////////////////////////////////////////////////////////////////////////
    // This really just calls the ImGui demo code. If you want to make your own
    // GUI, just go to that code and read it for reference. It also calls into
    // the giant imgui_demo.cpp demo file, which is a huge and wonderful imgui
    // reference. So I suspect checking that out if you want to do more 
    // interesting things.
    ///////////////////////////////////////////////////////////////////////////
    sample.Update();
    context.mClearColor = sample.mClearColor;

    ///////////////////////////////////////////////////////////////////////////
    // The rest of the loop is just the stuff you need to do to actually run
    // the hello-triangle example we set up above.
    ///////////////////////////////////////////////////////////////////////////
    // draw our first triangle
    gl::glUseProgram(shaderProgram);
    gl::glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
    //glDrawArrays(GL_TRIANGLES, 0, 6);
    gl::glDrawElements(gl::GL_TRIANGLES, 6, gl::GL_UNSIGNED_INT, 0);
  }

  return 0;
}