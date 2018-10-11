#pragma once

#include "includes-l1.hpp"
#include "drawStyle.hpp"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// app renderer

class BVHApp_renderer{

  public:
  
  GLFWwindow * renderTarget;
  BVHApp_renderer(GLFWwindow * t) : renderTarget(t){
  loadPickShader();
  }

  cgra::Program * c_prog; //current program

  cgra::Program pickProg;

  glm::mat4 projection;
  glm::mat4 view;

  // window stuff
  glm::vec2 m_viewportSize;

  int rtWid, rtHei;
 
  void checkSize(){
    glfwGetWindowSize(renderTarget, &rtWid, &rtHei);
  }

  //rendering options
  int highLight = -1;
  bool previewPick = false;

  void execute(std::vector<drawStyle> & target);

  void passUniforms(cgra::Program * p, uniforms * unfms);

  //dev
  void loadPickShader();

  void pickDraw(std::vector<drawStyle> & t);
  int pickTest( std::vector<drawStyle> target, glm::vec2 & m_mousePosition);

};
