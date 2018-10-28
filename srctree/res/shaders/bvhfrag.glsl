#version 430 core

//Layout
in vec3 fragPosition;
in vec3 fragNormal;

uniform mat4 viewMat;
uniform mat4 projectionMat; //might not use.. 

layout(std430, binding = 2) buffer vertbuff
{
  float pointFloats[];
};
layout(std430, binding = 3) buffer normalbuff
{
  float normalFloats[];
};
layout(std430, binding = 4) buffer uvbuff
{
  vec2 uvs[];
};
layout(std430, binding = 5) buffer tribuff
{
  uint tris[];
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

//Helper functions 
//Recompose vec3s
vec3 getPoint(unsigned int i){
  return vec3(
      pointFloats[i*3],
      pointFloats[i*3+1],
      pointFloats[i*3+2]
      );
}

vec3 getNorm(unsigned int i){
  return normalize(vec3(
      normalFloats[i*3],
      normalFloats[i*3+1],
      normalFloats[i*3+2]
      ));
}

//Control uniforrms
uniform mat4 viewRotation;
uniform vec3 viewPosition;

//Development switches, single element uniforms
const bool previewBBs = true;
out vec4 color;

//Lighting Regular Constants
const vec3 lightDir = vec3(0.1, 0.1, -1);
const vec3 ambientColor = vec3(0.3, 0.3, 0.3);
const vec3 diffuseColor = vec3(0.4, 0.4, 1.0);
const vec3 specColor    = vec3(0.2, 0.1, 0.1);
const float shininess = 16.0;

#define rot2(X) mat2(cos(X), -sin(X), sin(X), cos(X))

struct triHit{ 
  bool hits;
  vec3 hp;
  vec3 n;
  int elID;
  int matID;
  vec2 uv;
  float t; // how far along the ray

} theTri;
triHit ESCAPE = triHit(false, vec3(0), vec3(0),-1, -1, vec2(0));
#define FAR 99999999999

struct bvhHit{
  bool hits;
  vec3 hp;
  vec3 uv;
  float t;
} theBvhHit;

struct boxToTestWithin{
  int boxID;
  float t; // -1 For origin containers
} visit;

struct traceData{
  triHit bestHit;
  unsigned int treeDepth;
  float boxAccum; //Accumulate albedo of boxes;
} leafRay;

traceData leafRay;//All the state

struct bb_info{
  
}visitBB;
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

//Define some functions for intersecting primitives

vec3 bary(in vec3 a, in vec3 b, in vec3 c, in vec3 r){
//project a-r onto b-c
  vec3 rawbary =  vec3(
      r.x*(b.y*c.z-b.z*c.y) +
      r.y*(b.z*c.x-b.x*c.z) +
      r.z*(b.x*c.y-b.y*c.x),
      r.x*(c.y*a.z-c.z*a.y) +
      r.y*(c.z*a.x-c.x*a.z) +
      r.z*(c.x*a.y-c.y*a.x),
      r.x*(a.y*b.z-a.z*b.y) +
      r.y*(a.z*b.x-a.x*b.z) +
      r.z*(a.x*b.y-a.y*b.x)
      );

  float manhattan =
    /*
  rawbary.x * rawbary.x+
  rawbary.y * rawbary.y+
  rawbary.z * rawbary.z;
  return rawbary*rawbary/manhattan;*/
  rawbary.x+
  rawbary.y+
  rawbary.z;
  return rawbary/manhattan;

//  return normalize( rawbary  );
}

vec3 tribary( in unsigned int index, in vec3 p){
  return bary(getPoint(tris[index*3]),
                  getPoint(tris[index*3+1]),
                  getPoint(tris[index*3+2]),
                  p);
}
void testTriIndex(in ray r, in int index);
//Global variables

void cameraRaySetup();
  ray camera_ray;
  vec3 camera_p;
void bruteTris();
  float tri1 = 0;

void leafTrace( ray r, unsigned int head );
void main() {

  cameraRaySetup();
  // For all lights
    vec3 fragNormal;

   void bruteForceTris();
 
   // Get the illuminance traced by one ray in the simple
   // direct lighting case (the leaf of the ray tree)

   // RayTracing program:

   leafRay.triHit = ESCAPE;
   
   unsigned int headNode = 159;
   leafTrace(camera_ray,headNode);
      //Final compositing
   vec3 comp = vec3(0);

  comp += 1*fragNormal;
 
  float grey = 0;

  vec3 trgb  = vec3(0);
  comp += trgb;
  color = vec4(comp, 0);


}

/// Camera Setup //////////////////////////////
///////////////////////////////////////////////
void cameraRaySetup(){
   camera_p = viewPosition;
   camera_ray.d = normalize(vec3(fragPosition.xy,-1.)); //camera looks back at the origin by default
//adapt the viewmat#
   camera_ray.d *= mat3(
    vec3( viewRotation[0][0],
          viewRotation[0][1],
          viewRotation[0][2]),
    vec3( viewRotation[1][0],
          viewRotation[1][1],
          viewRotation[1][2]),
    vec3( viewRotation[2][0],
          viewRotation[2][1],
          viewRotation[2][2])
      );
    camera_ray.o = camera_p;
}

// Brute Force Tris ////////////////////////////
///////////////////////////////////////////////

void bruteTris(){
   // Brute force triangle testing

   vec3 accumNormCol = vec3(0);
   
   for (int i = 0; i < 80; i++){
  testTriIndex(camera_ray,i);
  tri1 = (theTri.hits==true) ? tri1+0.2 : tri1;
  if (theTri.hits){
    
    vec3 bary = tribary(i,theTri.hp);
      fragNormal =
      bary.x * getNorm(tris[i*3  ])+
      bary.y * getNorm(tris[i*3+1])+
      bary.z * getNorm(tris[i*3+2]);
   // fragNormal = normalize(getNorm(tris[i*3+1]));

    } 
  }
}
 
// Testing Methods

// Box Testing ////////////////////////

vec3 basis_transform_point( vec3 rp, mat3 b){
  return vec3(
      dot(rp,b[0]),
      dot(rp,b[1]),
      dot(rp,b[2])
      );
 
}
//TODO:test
// Slab Box Intersection //////////////////////////////
///////////////////////////////////////////////////////
void slabBox(unsigned int boxID, ray r){
vec3 relp;
vec3 tro;
vec3 trd;

  theBvhHit.hits = false;

  relp = r.o - getBoxO(boxID);
  mat3 basis = getBB_Basis (boxID);

  // Transform to basis first. Test against axis planes.
    vi = normalize(basis[0]);
    vj = normalize(basis[1]);
    vk = normalize(basis[2]);
    
  tro = basis_transform_point(relp, mat3(vi,vj,vk));
  trd = basis_transform_point(r.d, mat3(vi,vj,vk));

  float   mint_i, maxt_i,
          mint_j, maxt_j,
          mint_k, maxt_k;
     
  // Plane intersections
  //dot the ray dir with the basis;

  float dR_i = trd.x;
  float dR_j = trd.y;
  float dR_k = trd.z;

  //dot the ray origin with the basis;
  float dRor_i = tro.x;
  float dRor_j = tro.y;
  float dRor_k = tro.z;

  //basis lengths should be prepared earlier. TODO: move this.
  vec3 dims = vec3(
    length(basis[0]),
    length(basis[1]),
    length(basis[2])
    );

//Intersections
if (dR_i != 0)
  mint_i = dRor_i / dR_i;
  maxt_i = mint_i + dims[0]/dR_i;
if (dR_j != 0)
  mint_j = dRor_j / dR_j;
  maxt_j = mint_j + dims[1]/dR_j;
if (dR_k != 0)
  mint_k = dRor_k / dR_k;
  maxt_k = mint_k + dims[2]/dR_k;

  float mint = -FAR;
  float maxt = FAR;

//Clipping - forward
if (dR_i>0){ //
  mint = max(mint, mint_i); 
  maxt = min(maxt, maxt_i); 
} 
if (dR_j>0){ // 
  mint = max(mint, mint_j); 
  maxt = min(maxt, maxt_j); 
} 
if (dR_k>0){ // 
  mint = max(mint, mint_k); 
  maxt = min(maxt, maxt_k); 
}
//Clipping - reverse
if (dR_i>0){ //
  mint = max(mint, maxt_i); 
  maxt = min(maxt, mint_i); 
} 
if (dR_j<0){ // 
  mint = max(mint, maxt_j); 
  maxt = min(maxt, mint_j); 
} 
if (dR_k<0){ // 
  mint = max(mint, maxt_k); 
  maxt = min(maxt, mint_k); 
}

  theBvhHit.hits = (maxt>=mint);
  theBvhHit.t = mint;
}

//TODO:test
////////////////////////////////////////
// Box Contains Point ///////////////////
bool inside;
void boxcontains(unsigned int boxID, vec3 p){
  mat3 basis = getBB_Basis (boxID);
  vec3 dims = getBB_Dims(boxID);
  
  vec3 relp = p - getBoxO (boxID);


  vec3 ba,bb,bc;
    vi = normalize(basis[0]);
    vj = normalize(basis[1]);
    vk = normalize(basis[2]);
// Todo: Investigate whether constructing individual floats
// and testing them to break early is faster
    vec3 tp = basis_transform_point(relp, mat3(vi,vj,vk));

    inside = (
      0 < tp.x && tp.x < dims.x &&
      0 < tp.y && tp.x < dims.y &&
      0 < tp.z && tp.x < dims.z
      );

}

// Triangle /////////////////////////////
void testTriIndex(in ray r, in int index){

//Get the tri points
  vec3 a = getPoint(tris[index*3]);
  vec3 b = getPoint(tris[index*3+1]);
  vec3 c = getPoint(tris[index*3+2]);

  //Get the pyramid vecs
  vec3 rtoA = a-r.o;
  vec3 rtoB = b-r.o;
  vec3 rtoC = c-r.o;

  //Middle point
  vec3 mid = rtoA+rtoB+rtoC;

  float drFaceAB = dot(r.d, cross(rtoB,rtoA));
  float drFaceBC = dot(r.d, cross(rtoC,rtoB));
  float drFaceCA = dot(r.d, cross(rtoA,rtoC));

  theTri.hits = (drFaceAB > 0 && drFaceBC> 0 && drFaceCA > 0);

  theTri.hits = (drFaceAB == 1 || drFaceBC == 1 || drFaceCA == 1 || dot(mid,r.d) < 0) ?
  false : theTri.hits;

  // Plane intersection
  theTri.n = normalize( cross( b-a, c-a ));
  theTri.hp = r.o + r.d * dot(theTri.n,(a -r.o))/dot(theTri.n,r.d);
}

//TODO:test
   /////////////////////////////////////////////////////
   // Trace Function   /////////////////////////////////
   /////////////////////////////////////////////////////
void leafTrace( ray r, head ) {
   
  //leafTrace writes to the global leafRay struct.

    visit.boxID = head ; //Begin beyond the root

   //Make a stack of destinations 
   unsigned int stackCounter = 0;
   boxToTestWithin visitStack[32]; // TODO resolve maximum stack depth


   // PUSH TO ORIGIN PHASE ////////////////////////////////
     //walk to the one which contains the origin.
    while( boxcontains (visit.boxID,r.o) ){
    
      if topo[visit.boxID*2] == -1; //triangle case - origin is in one prim's bounding box
      break;

      //else its a parent node, 
      //push its smaller one to the stack and visit the first.

      slabBox(topo[visit.boxID*2+1]);
      boxToTestWithin visitStack[stackCounter] = theBvhHit;
      visit = boxToTestWithin(topo[visit.boxID*2],t);
      stackCounter ++;
     
      //or theres no root.)

    }
    // Now visiting at a primitive node, or visiting at a non origin
    // containing node.

    // This node must be tested. Push it to the stack.
    slabBox(visit.boxID);
    if (theBvhHit.hits) visitStack[stackCounter ++] = visit;
    
   // INTERSECTION PHASE ////////////////////////////////////
   // Stack built. Start intersecting.
    while ( stackCounter > 0 ){
    // Pop a box that I know I intersect with.
    // The stackCounter is decremented on every lap.
      visit = visitStack[-1+stackCounter]; stackCounter--;
    // Is it beyond the closest media ?
      if (visit.t < bestHit.t){
        continue; 
      }

    // Primitive case
      testTriIndex(topo[visit.boxID*2+1]);
      if(theTri.t <leafRay.bestHit.t &&  
         theTri.t > 0){
        leafRay.bestHit = theTri;
        continue;
      }
   // Parent case; Intersect a box
      unsigned int small = topo[visit.boxID * 2];
      unsigned int large = topo[visit.boxID * 2 + 1];
    // Intersection cases
    // Push the bigger box last, to test it first.
    // (Small boxes are more likely to be further than the best hit)
      slabBox(small,r);
      if(theBvhHit.hits){
       visitStack[stackCounter++] = boxToTestWithin(small,theBvhHit.t);
      }
      slabBox(large,r);
      if(theBvhHit.hits){
       visitStack[stackCounter++] = boxToTestWithin(large,theBvhHit.t);
      }
    } // Traversal escaped the tree. Got the best hit.
//////////////////////////////////////////////////////////
}


