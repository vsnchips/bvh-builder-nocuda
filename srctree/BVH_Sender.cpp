#include "includes-l1.hpp"
#include "bvhApp.hpp"

int sendBVH( BVH & theBVH, GLFWwindow * ctx){

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
   size_t size = sizeof(glm::vec3) * theBVH.bvh_buffs.verts.size();
   const void * buffHead =  reinterpret_cast<const void *>(&(theBVH.bvh_buffs.verts[0]));
  glBufferData(GL_SHADER_STORAGE_BUFFER, size, buffHead, GL_DYNAMIC_DRAW);

  glBindBuffer(GL_SHADER_STORAGE_BUFFER, normalbuff);
   size = sizeof(glm::vec3) * theBVH.bvh_buffs.normals.size();
   buffHead =  reinterpret_cast<const void *>(&(theBVH.bvh_buffs.normals[0]));
  glBufferData(GL_SHADER_STORAGE_BUFFER, size, buffHead, GL_DYNAMIC_DRAW);

  glBindBuffer(GL_SHADER_STORAGE_BUFFER, uvbuff);
   size = sizeof(glm::vec2) * theBVH.bvh_buffs.uvs.size();
   buffHead =  reinterpret_cast<const void *>(&(theBVH.bvh_buffs.uvs[0]));
  glBufferData(GL_SHADER_STORAGE_BUFFER, size, buffHead, GL_DYNAMIC_DRAW);

  glBindBuffer(GL_SHADER_STORAGE_BUFFER, tribuff);
   size = sizeof(int) * theBVH.bvh_buffs.trispecs.size();
   buffHead =  reinterpret_cast<const void *>(&(theBVH.bvh_buffs.trispecs[0]));
  glBufferData(GL_SHADER_STORAGE_BUFFER, size, buffHead, GL_DYNAMIC_DRAW);

  glBindBuffer(GL_SHADER_STORAGE_BUFFER, topobuff);
   size = sizeof(int) * theBVH.bvh_buffs.topo.size();
   buffHead =  reinterpret_cast<const void *>(&(theBVH.bvh_buffs.topo[0]));
  glBufferData(GL_SHADER_STORAGE_BUFFER, size, buffHead, GL_DYNAMIC_DRAW);

  glBindBuffer(GL_SHADER_STORAGE_BUFFER, bb_buff);
   size = sizeof(glm::vec3) * theBVH.bvh_buffs.bb_specs.size();
   buffHead =  reinterpret_cast<const void *>(&(theBVH.bvh_buffs.bb_specs[0]));
  glBufferData(GL_SHADER_STORAGE_BUFFER, size, buffHead, GL_DYNAMIC_DRAW);

  gl_errorFlush("send BVH ");
  return 0;

}
