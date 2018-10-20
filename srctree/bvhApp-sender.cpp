#include "includes-l1.hpp"
#include "bvhApp.hpp"

void BVH::sendScene(){

  GLuint vertexbuff;
  GLuint normalbuff;
  GLuint uvbuff;
  GLuint tribuff;
  GLuint topobuff;
  GLuint bb_buff;

  glDeleteBuffers(1,&vertexbuff);
  glDeleteBuffers(1,&normalbuff);
  glDeleteBuffers(1,&uvbuff);
  glDeleteBuffers(1,&tribuff);
  glDeleteBuffers(1,&topobuff);
  glDeleteBuffers(1,&bb_buff);

  glGenBuffers(1,&vertexbuff);
  glGenBuffers(1,&normalbuff);
  glGenBuffers(1,&uvbuff);
  glGenBuffers(1,&tribuff);
  glGenBuffers(1,&topobuff);
  glGenBuffers(1,&bb_buff);

  glBindBuffer(GL_SHADER_STORAGE_BUFFER, vertexbuff);
   size_t size = sizeof(glm::vec3) * theBVH.verts.size();
   const void * buffHead =  reinterpret_cast<const void *>(&(theBVH.verts[0]));
  glBufferData(GL_SHADER_STORAGE_BUFFER, sizei, buffHead, GL_DYNAMIC_DRAW);

  glBindBuffer(GL_SHADER_STORAGE_BUFFER, normalbuff);
   size = sizeof(glm::vec3) * theBVH.normals.size();
   buffHead =  reinterpret_cast<const void *>(&(theBVH.normals[0]));
  glBufferData(GL_SHADER_STORAGE_BUFFER, sizei, buffHead, GL_DYNAMIC_DRAW);

  glBindBuffer(GL_SHADER_STORAGE_BUFFER, uvbuff);
   size = sizeof(glm::vec2) * theBVH.uvs.size();
   buffHead =  reinterpret_cast<const void *>(&(theBVH.uvs[0]));
  glBufferData(GL_SHADER_STORAGE_BUFFER, sizei, buffHead, GL_DYNAMIC_DRAW);

  glBindBuffer(GL_SHADER_STORAGE_BUFFER, tribuff);
   size = sizeof(int) * theBVH.trispecs.size();
   buffHead =  reinterpret_cast<const void *>(&(theBVH.trispecs[0]));
  glBufferData(GL_SHADER_STORAGE_BUFFER, sizei, buffHead, GL_DYNAMIC_DRAW);

  glBindBuffer(GL_SHADER_STORAGE_BUFFER, topobuff);
   size = sizeof(int) * theBVH.topo.size();
   buffHead =  reinterpret_cast<const void *>(&(theBVH.topo[0]));
  glBufferData(GL_SHADER_STORAGE_BUFFER, sizei, buffHead, GL_DYNAMIC_DRAW);

  glBindBuffer(GL_SHADER_STORAGE_BUFFER, bb_buff);
   size = sizeof(glm::vec3) * theBVH.bb_specs.size();
   buffHead =  reinterpret_cast<const void *>(&(theBVH.bb_specs[0]));
  glBufferData(GL_SHADER_STORAGE_BUFFER, sizei, buffHead, GL_DYNAMIC_DRAW);


}
