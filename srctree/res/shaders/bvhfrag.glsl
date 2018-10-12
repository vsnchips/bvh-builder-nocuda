//version 150 core
#version 430 core

in vec3 fragPosition;
in vec3 fragNormal;

//layout (location = 0)
  
out vec4 color;

//Lighting Regular Constants
const vec3 lightDir = vec3(0.1, 0.1, -1);
const vec3 ambientColor = vec3(0.3, 0.3, 0.3);
const vec3 diffuseColor = vec3(0.4, 0.4, 1.0);
const vec3 specColor    = vec3(0.2, 0.1, 0.1);
const float shininess = 16.0;

uniform mat4 viewMat;
uniform mat4 projectionMat; //might not use.. 

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



void main() {

   vec3 comp;
   
   comp = fragPosition;
  //  comp = vec3(0,0,1);

    color = vec4(comp, 0.3);

}
