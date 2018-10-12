#include "BVHRenderer.hpp"

//IMPLEMENTATIONS//////////////////////////////////
//TODO: move to seperate source

void BVHRenderer::setView(){}
void BVHRenderer::setLights(){}
void BVHRenderer::updateEnvironment(){}
void BVHRenderer::execute(){

  glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
  glEnable(GL_DEPTH_TEST);

  /*if (glfwGetCurrentContext() != context) glfwMakeContextCurrent(context);
  GLint current; glGetIntegerv(GL_CURRENT_PROGRAM,&current);
  if (current != bvh_program) glUseProgram(bvh_program);*/

  glUseProgram(bvh_program);
 
  //
  glBindVertexArray(bvh_quad_vao);
  gl_errorFlush("bindvao");
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  gl_errorFlush("polygonmode");
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
  gl_errorFlush("drawElements");

}

//Constructors, Initialisers
void BVHRenderer::initQuad(){
  //Quad GL Buffers
  glDeleteVertexArrays(1,&bvh_quad_vao);
  glDeleteBuffers(1,&bvh_quad_vbo);
  glDeleteBuffers(1,&bvh_quad_ibo);
  glGenVertexArrays(1,&bvh_quad_vao);
  glBindVertexArray(bvh_quad_vao);
  glGenBuffers(1,&bvh_quad_vbo);
  glBindBuffer(GL_ARRAY_BUFFER, bvh_quad_vbo);
  glGenBuffers(1,&bvh_quad_ibo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,bvh_quad_ibo);

  //Quad data 
  std::vector<glm::vec3> points; points.clear();
  points.push_back(glm::vec3(-1,-1,0));
  points.push_back(glm::vec3(1,-1,0));
  points.push_back(glm::vec3(1,1,0));
  points.push_back(glm::vec3(-1,1,0));
  std::vector<unsigned int> indices; indices.clear();
  indices.push_back(0);
  indices.push_back(1);
  indices.push_back(2);
  indices.push_back(2);
  indices.push_back(3);
  indices.push_back(0);
 
  size_t size = sizeof(glm::vec3) * 4;
  const void * buffHead =  reinterpret_cast<const void *>(&(points[0]));
  glBufferData(GL_ARRAY_BUFFER, size, buffHead, GL_STATIC_DRAW );

  size = sizeof(unsigned int) * 6;
  buffHead = reinterpret_cast<const void *> (&indices[0]);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, buffHead, GL_STATIC_DRAW); 

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(glm::vec3),0);
}
