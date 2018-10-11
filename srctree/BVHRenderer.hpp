#pragma once

#include "includes-l1.hpp"
#include "BVH.hpp"
class BVHRenderer{

  public:
    GLFWwindow * context
  
  GLuint bvh_program;
  
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
  
}

//TODO: move to seperate source

BVHRenderer::setView(){}
BVHRenderer::setLights(){}
BVHRenderer::updateEnvironment(){}
BVHRenderer::execute(){

  if (glfwGetCurrentContext != context) glfwMakeContextCurrent(context);
  GLuint current; glGet(GL_CURRENT_PROGRAM,&current);
  if (current != bvh_program) glUseProgram(bvh_program);
 
  //
  glBindVertexArray(quad_vao);
  gl_errorFlush("bindvao");
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  gl_errorFlush("polygonmode");
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
  gl_errorFlush("drawElements");

}

//Constructors, Initialisers

void BVHRenderer::initQuad(){

  glDeleteVertexArrays(1,&quad_vao);
  glDeleteBuffers(1,&quad_vbo);
  glDeleteBuffers(1,&quad_ibo);
  glGenVertexArrays(1,&quad_vao);
  glBindVertexArray(quad_vao);
  glGenBuffers(1,&quad_vbo);
  glBindBuffer(GL_ARRAY_BUFFER, quad_vbo);
  glGenBuffers(1,&quad_ibo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,quad_ibo);

  vector<vec3> points; points.clear();
  points.push_back(vec3(-1,-1,0));
  points.push_back(vec3(1,-1,0));
  points.push_back(vec3(1,1,0));
  points.push_back(vec3(-1,1,0));
  vector<unsigned int> indices; indices.clear();
  indices.push_back(0);
  indices.push_back(1);
  indices.push_back(2);
  indices.push_back(2);
  indices.push_back(3);
  indices.push_back(0);
 
  size_t size = sizeof(vec3) * 4;
  const void * buffHead =  reinterpret_cast<const void *>(&(points[0]));
  glBufferData(GL_ARRAY_BUFFER, size, buffHead, GL_STATIC_DRAW );


  size = sizeof(unsigned int) * 6;
  buffHead = reinterpret_cast<const void *> (&indices[0]);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, buffHead, GL_STATIC_DRAW); 

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(vec3),0);
  

}
