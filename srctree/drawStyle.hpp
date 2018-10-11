#pragma once

#include "includes-l1.hpp"

//each composite object returns an array of draw styles
// instead of each object overriding a draw function,
// the app implements a draw function that iterates
// over the object's array of draw styles.

//each vao/mesh has a drawStyle

struct uniforms{
  std::map<std::string, int> i1;
  std::map<std::string, float> f1;
  std::map<std::string, glm::vec3> f3;
  std::map<std::string, glm::mat4> m4;
};

class drawStyle{
  //all ptrs can be null
  //this allows drawStyles with only uniform settings
  //to be included like optional lines
public:
  GLuint mode=-1;
  GLuint vao;
  int primCount;
  std::string tag;
  cgra::Program * prog = nullptr;
  uniforms unfms;
  glm::mat4 wmMat;
  cgra::Mesh * m_mesh = nullptr;    // Draw meshes as they appear in the lineStream
  std::vector<glm::vec2> * partBuffer; //particle buffer
  

  void clear(){
  prog = nullptr;
  m_mesh = nullptr;
  unfms.i1.clear();
  unfms.f1.clear();
  unfms.f3.clear();
  unfms.m4.clear();

}


 void  putProjMat( glm::mat4 pm){
  unfms.m4["projectionMat"] = pm;
 }

 void putViewMat( glm::mat4 vm){
  unfms.m4["viewMat"] = vm;
 }

 void putModelMat( glm::mat4 wmm){
  unfms.m4["modelMat"] = wmm;
 }

  
  
};


typedef std::vector<drawStyle> drawList;
