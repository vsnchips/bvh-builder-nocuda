#version 430 core

in vec3 fragPosition;
in vec3 fragNormal;

uniform mat4 viewMat;
uniform mat4 projectionMat; //might not use.. 

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
  unsigned int tris[];
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



//Switches, single element uniforms
const bool previewBBs = true;
//layout (location = 0)
//Test Buffers


/*/The BVH Sends shader storage buffers of:

  Vertices
  Normals
  MATids
  primIdentifiers // for development
  Triangle Specs

  BVH Pointer Tree

  Theses buffers all have associated:
  uniform int count;

 */



//Test Buffers:
const vec3 test_vertices[18] = vec3[]
(
 vec3(0.0,0.0,0.0),
 vec3(0.0,0.0,0.0),
 vec3(0.0,0.0,0.0),
 vec3(0.0,0.0,0.0),
 vec3(0.0,0.0,0.0),
 vec3(0.0,0.0,0.0),
 vec3(0.0,0.0,0.0),
 vec3(0.0,0.0,0.0),
 vec3(0.0,0.0,0.0),
 vec3(0.0,0.0,0.0),
 vec3(0.0,0.0,0.0),
 vec3(0.0,0.0,0.0),
 vec3(0.0,0.0,0.0),
 vec3(0.0,0.0,0.0),
 vec3(0.0,0.0,0.0),
 vec3(0.0,0.0,0.0),
 vec3(0.0,0.0,0.0),
 vec3(0.0,0.0,0.0)
 );

const int test_indices[18] = int[]
(
 0,1,2,
 3,4,5,
 6,7,8,
 9,10,11,
 12,13,14,
 15,16,17
 );


out vec4 color;

//Lighting Regular Constants
const vec3 lightDir = vec3(0.1, 0.1, -1);
const vec3 ambientColor = vec3(0.3, 0.3, 0.3);
const vec3 diffuseColor = vec3(0.4, 0.4, 1.0);
const vec3 specColor    = vec3(0.2, 0.1, 0.1);
const float shininess = 16.0;

#define rot2(X) mat2(cos(X), -sin(X), sin(X), cos(X))

struct traceData{
  bool hits;
  vec3 hp;
  vec3 incident;
  vec3 normal;
  int elID;
  int matID;
  vec2 uv;
}escape;

traceData ESCAPE = traceData(false, vec3(0), vec3(0), vec3(0), -1, -1, vec2(0));

struct triangle{
  vec3 pa;
  vec3 pb;
  vec3 pc;
  vec3 normA;
  vec3 normB;
  vec3 normC;
  vec2 uvA;
  vec2 uvB;
  vec2 uvC;
  int matID;

}dummyTri;


struct ray{
  vec3 o;
  vec3 p;

  vec3 d;
}eyeRay;

struct bvhElement{
  int subA;  //Contained Elm A
  int subB;  //Contained Elm B
};
//Test Data

triangle testPrims[4];

bvhElement lookUpBVH(){
  bvhElement ret;
  return ret;
}

bool isElThere( int el){
  return false;  
};

bool isElPrim( int el){
  return true;
};

//Define some functions for finding an index to a primitives
traceData traceBVNode(ray r,int element){
  traceData res;
  return res;
}

//Define some functions for intersecting primitives
traceData tri( ray r, triangle t){
  vec3 trinorm = normalize(cross(t.pb-t.pa,t.pc-t.pb));
  vec3 norm_Centre = normalize(cross(t.pb-t.pa,t.pc-t.pa));
  float d = dot( t.pa - r.o, norm_Centre )/dot(r.d,norm_Centre);
  
  // if t.pe ray looks at.pt.pe t.piangle
  t.pa = t.pa - r.o;
  t.pb = t.pb - r.o;
  t.pc = t.pc - r.o;

  vec3 toA = t.pa - r.o;
  vec3 toB = t.pb - r.o;
  vec3 toC = t.pc - r.o;

  bool bounded = (
  dot ( r.d , cross(toB, toA)) > 0  &&
  dot ( r.d , cross(toC, toB)) > 0  &&
  dot ( r.d , cross(toA, toC)) > 0 
  
  );
  
  traceData res;
  res.hits = bounded ;
  res.incident = r.d;
  res.normal = norm_Centre;
  res.matID = t.matID;
  //todo:: barycentric uv coords;
  res.hp = r.o+d*r.d;
  return res;
}


bool testBB(int i){
  return true;
}

/*
traceData closeHit(){

  traceData close;
  close

#define MAX_VISIT_STACK 128
int vstack[MAX_VISIT_STACK];
int si = 0; // stack index;

while(si>=0){

  if(te){
    if(){
    }
    else{
    }
  }else{
  }


}

return close;
}
*/

traceData bvhTrace(ray r){
  traceData close = ESCAPE;
 
  #define MAX_STACK 128
  int vstack[MAX_STACK];
  int si = 0;
  
  while( si >= 0)


  return close;

}

traceData testTriIndex(ray r, unsigned int index){

  traceData retr;
//Get the tri points
  vec3 a = points[tris[index*3]];
  vec3 b = points[tris[index*3+1]];
  vec3 c = points[tris[index*3+2]];

/*  a=points[0];
  b=points[1];
  c=points[2];
*/
  //Get the pyramid vecs
  vec3 rtoA = a-r.o;
  vec3 rtoB = b-r.o;
  vec3 rtoC = c-r.o;

  float drFaceAB = dot(r.d, cross(rtoB,rtoA));
  float drFaceBC = dot(r.d, cross(rtoC,rtoB));
  float drFaceCA = dot(r.d, cross(rtoA,rtoC));

  if(drFaceAB > 0 && drFaceBC> 0 && drFaceCA > 0){
  //if(drFaceAB < 0 && drFaceCA < 0 && drFaceCA < 0){
    //hits
    retr.hits = true;
  }
  else retr.hits=false;
  
  return retr;
}

void main() {

   vec3 camera_p = vec3(0.,-0.,5);
   ray camera_ray;
   camera_ray.d = normalize(vec3(fragPosition.xy,1.));
   camera_ray.d = camera_ray.d.xyz;
   camera_ray.o = camera_p;

   //Here goes some tracing

   //No distance functions to get the normals. They must be interpolated from the triangles barycentrically.

   // For all lights
//   traceData 

   
  
  vec3 comp = vec3(0);
  //comp = fragPosition* 0.3;
  //  comp = vec3(0,0,1);

  comp.b = points[0].r;
  comp.b = bbs[13].r;
 
  float grey = 0;
  float tri1 = (testTriIndex(camera_ray,3).hits==true) ? 0.9 : 0;
  grey += tri1;

  vec3 trgb  = vec3(0);
  //trgb =points[1];

  comp = vec3(grey);
  comp += trgb;
  color = vec4(comp, 0);

}
