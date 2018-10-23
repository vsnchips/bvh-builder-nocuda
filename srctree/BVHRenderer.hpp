#pragma once

#include "includes-l1.hpp"
#include "BVH.hpp"

//HEADER
class BVHRenderer{

  public:
  GLFWwindow * context;
  
  GLuint bvh_comp_program;
  GLuint ssbo_bbs;

  GLuint bvh_frag_program;

  GLuint bvh_quad_vao;
  GLuint bvh_quad_vbo;
  GLuint bvh_quad_ibo;

  //Constructors, initialisers:
  BVHRenderer(GLFWwindow * ctx) : context(ctx){}
  void initQuad();

  //updates: camera, lights, procedural uniforms etc.
  void setView();
  void setLights();
  void updateEnvironment();

  void execute();
  
};

//End header 
