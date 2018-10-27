#version 430
//#pragma kernel CS_main

layout(std430, binding = 2) buffer vertbuff
{
  vec3 points[];
};
layout(std430, binding = 3) buffer normalbuff
{
  vec3 normals[];
};
layout(std430, binding = 4) buffer uvbuff
{
  vec2 uvs[];
};
layout(std430, binding = 5) buffer tribuff
{
  int tris[];
};
layout(std430, binding = 6) buffer topobuff
{
  int headNode;
  int topo[];
};
layout(std430, binding = 7) buffer bbsbuff
{
  vec3 bbs[];
};


//uniform mat4 viewMat;
//uniform mat4 projectionMat; //might not use.. 


layout (local_size_x = 1, local_size_y = 1, local_size_z=1) in;

//void CS_main(){
void main(){
//Do some work

//  normals[2].r = 0.15;

  /*
  points[0] = vec3(1,-0,3);
  points[1] = vec3(2,1.2,0);
  points[2] = vec3(-1.5,1.2,1);

  tris[3]=0;
  tris[4]=1;
  tris[5]=2;
  */

}
