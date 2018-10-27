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

   size_t size; 
   const void * buffHead;
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, vertexbuff);
   size = sizeof(glm::vec3) * theBVH.bvh_buffs.verts.size();
   buffHead =  reinterpret_cast<const GLfloat *>(&(theBVH.bvh_buffs.verts[0]));
  glBufferData(GL_SHADER_STORAGE_BUFFER, size, buffHead, GL_STATIC_DRAW);
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER,2,vertexbuff);

  glBindBuffer(GL_SHADER_STORAGE_BUFFER, normalbuff);
   size = sizeof(glm::vec3) * theBVH.bvh_buffs.normals.size();
   buffHead =  reinterpret_cast<const GLfloat *>(&(theBVH.bvh_buffs.normals[0]));
  glBufferData(GL_SHADER_STORAGE_BUFFER, size, buffHead, GL_STATIC_DRAW);
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER,3,normalbuff);

  glBindBuffer(GL_SHADER_STORAGE_BUFFER, uvbuff);
   size = sizeof(glm::vec2) * theBVH.bvh_buffs.uvs.size();
   buffHead =  reinterpret_cast<const void *>(&(theBVH.bvh_buffs.uvs[0].x));
  glBufferData(GL_SHADER_STORAGE_BUFFER, size, buffHead, GL_STATIC_DRAW);
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER,4,uvbuff);

  glBindBuffer(GL_SHADER_STORAGE_BUFFER, tribuff);
   size = sizeof(unsigned int) * theBVH.bvh_buffs.trispecs.size();
   buffHead =  reinterpret_cast<const GLuint *>(&(theBVH.bvh_buffs.trispecs[0]));
  glBufferData(GL_SHADER_STORAGE_BUFFER, size, buffHead, GL_STATIC_DRAW);
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER,5,tribuff);

  glBindBuffer(GL_SHADER_STORAGE_BUFFER, topobuff);
   size = sizeof(int) * theBVH.bvh_buffs.topo.size();
   buffHead =  reinterpret_cast<const GLint *>(&(theBVH.bvh_buffs.topo[0]));
  glBufferData(GL_SHADER_STORAGE_BUFFER, size, buffHead, GL_STATIC_DRAW);
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER,6,topobuff);

  glBindBuffer(GL_SHADER_STORAGE_BUFFER, bb_buff);
   size = sizeof(glm::vec3) * theBVH.bvh_buffs.bb_specs.size();
   buffHead =  reinterpret_cast<const GLfloat *>(&(theBVH.bvh_buffs.bb_specs[0]));
  glBufferData(GL_SHADER_STORAGE_BUFFER, size, buffHead, GL_STATIC_DRAW);
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER,7,bb_buff);


  //std::cout << "TRIANGLE INDEX BUFFER: " << theBVH.bvh_buffs.trispecs;
  std::cout << "Uploaded BVH";

  gl_errorFlush("send BVH ");
  return 0;

}
