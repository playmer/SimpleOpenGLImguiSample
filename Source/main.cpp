// dear imgui: standalone example application for GLFW + OpenGL 3, using programmable pipeline
// If you are new to dear imgui, see examples/README.txt and documentation at the top of imgui.cpp.
// (GLFW is a cross-platform general purpose library for handling windows, inputs, OpenGL/Vulkan graphics context creation, etc.)

#include <stdio.h>
#include <iostream>

#include "ImGuiSample.hpp"
#include "ApplicationContext.hpp"

int main(int, char**)
{
  SOIS::ApplicationInitialization();
  SOIS::ApplicationContext context;

  SOIS::ImGuiSample sample;

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
  // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
  gl::glBindVertexArray(VAO);

  gl::glBindBuffer(gl::GL_ARRAY_BUFFER, VBO);
  gl::glBufferData(gl::GL_ARRAY_BUFFER, sizeof(vertices), vertices, gl::GL_STATIC_DRAW);

  gl::glBindBuffer(gl::GL_ELEMENT_ARRAY_BUFFER, EBO);
  gl::glBufferData(gl::GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, gl::GL_STATIC_DRAW);

  gl::glVertexAttribPointer(0, 3, gl::GL_FLOAT, gl::GL_FALSE, 3 * sizeof(float), (void*)0);
  gl::glEnableVertexAttribArray(0);

  // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
  gl::glBindBuffer(gl::GL_ARRAY_BUFFER, 0);

  // remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
  //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
  // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
  gl::glBindVertexArray(0);

  // Main loop
  while (context.Update())
  {
    sample.Update();

    context.mClearColor = sample.mClearColor;

    // draw our first triangle
    gl::glUseProgram(shaderProgram);
    gl::glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
    //glDrawArrays(GL_TRIANGLES, 0, 6);
    gl::glDrawElements(gl::GL_TRIANGLES, 6, gl::GL_UNSIGNED_INT, 0);
  }

  return 0;
}