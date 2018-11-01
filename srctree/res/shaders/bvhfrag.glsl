#version 430 core
//#pragma optimise(off)
#define MAX_TRAVERSAL_STEPS 512 
//Layout
in vec3 fragPosition;
out vec4 color;

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
  /*Layout:
  0-2: Origin
  3-5: Dimensions
  6-14: Orthonormal basis
  */
  float bbs[];
};

//Helper functions 
//Recompose vec3s

//vec3 getBB_Origin(unsigned int i){ return vec3(-0.51,-0.51,-0.49);}
//vec3 getBB_Dims(unsigned int i){ return vec3( 0.3, 0.3, 0.3 );}
/*vec3 getBBi(unsigned int i){ return vec3(1,0,0);}
vec3 getBBj(unsigned int i){ return vec3(0,1,0);}
vec3 getBBk(unsigned int i){ return vec3(0,0,1);}
*/

vec3 getBB_Origin(unsigned int i){
  return vec3(
      bbs[i*15+0],
      bbs[i*15+1],
      bbs[i*15+2] );
}
vec3 getBB_Dims(unsigned int i){
  return vec3(
      bbs[i*15+3],
      bbs[i*15+4],
      bbs[i*15+5] );
}
vec3 getBBi(unsigned int i){
  return vec3(
      bbs[i*15+6],
      bbs[i*15+7],
      bbs[i*15+8]);
}

vec3 getBBj(unsigned int i){
  return vec3(
      bbs[i*15+9],
      bbs[i*15+10],
      bbs[i*15+11]);
}

vec3 getBBk(unsigned int i){
  return vec3(
      bbs[i*15+12],
      bbs[i*15+13],
      bbs[i*15+14]);
}

mat3 getBB_Basis(unsigned int i){
  return mat3(
     getBBi(i), 
     getBBj(i), 
     getBBk(i) 
   );
}

//Vertex Attributes
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

//Lighting Regular Constants
const vec3 lightDir = vec3(0.1, 0.1, -1);
const vec3 ambientColor = vec3(0.3, 0.3, 0.3);
const vec3 diffuseColor = vec3(0.4, 0.4, 1.0);
const vec3 specColor    = vec3(0.2, 0.1, 0.1);
const float shininess = 16.0;

#define rot2(X) mat2(cos(X), -sin(X), sin(X), cos(X))
#define FAR 9999999

struct triHit{ 
  bool hits;
  vec3 hp;
  vec3 n;
  int elID;
  int matID;
  vec2 uv;
  float t; // how far along the ray

} theTri;
const triHit ESCAPE = triHit(false, vec3(0), vec3(0),-1, -1, vec2(0),FAR);

struct bvhHit{
  bool hits;
  vec3 hp;
  vec2 uv;
  float t;
} theBvhHit;

const bvhHit BVH_ESCAPE = bvhHit(false,vec3(0),vec2(0),FAR);

struct traceData{
  triHit bestHit;
  unsigned int treeDepth;
  float boxAccum; //Accumulate albedo of boxes;
} leafRay; 
 
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

//Globals and forward declarations

void testTriIndex(in ray r, in int index);
vec3 fragNormal;
vec3 leafTraceCol;
void cameraRaySetup();
ray camera_ray;
vec3 camera_p;
void bruteForceTris(uint count);
  
  float tri1 = 0;

void leafTrace( ray r, unsigned int head );

void main() {

   cameraRaySetup();
  // For all lights

   // RayTracing program:
   
   //unsigned int headNode = 3600-16-2  ;
   unsigned int headNode = 2*1355-2  ;
//   headNode = iHead;

   leafTraceCol = vec3(0);
   leafTrace(camera_ray,headNode);
   
   if(leafRay.bestHit.hits){
     // A fresnel event interface has been found.
     // It is a point on a triangle.
     //Process it :
     //Interpolate its normals.
     int i = leafRay.bestHit.elID;
     vec3 bary = tribary(i, leafRay.bestHit.hp);
     leafTraceCol +=
      bary.x * getNorm(tris[i*3  ]) +
      bary.y * getNorm(tris[i*3+1]) +
      bary.z * getNorm(tris[i*3+2])
      ;
  }
  else{
  leafTraceCol = vec3(leafRay.boxAccum)*0.22 ;
  }
  
 //  bruteForceTris( 400 );

   //Final compositing
   vec3 comp = vec3(0);
   comp += leafTraceCol;//
  float grey = 0;
  // vec3 trgb = getPoint(0);
  //comp += trgb;
  color = vec4(comp, 0);

}

/// Camera Setup //////////////////////////////
///////////////////////////////////////////////
void cameraRaySetup(){
   camera_p = viewPosition;
   camera_ray.d = normalize(vec3(fragPosition.xy,-1.5)); //camera looks back at the origin by default
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

void bruteForceTris(uint count ){
   // Brute force triangle testing

   vec3 accumNormCol = vec3(0);
   
   for (int i = 0; i < count ; i++){
  testTriIndex(camera_ray,i);
  if (theTri.hits){
    
    vec3 bary = tribary(i,theTri.hp);
    leafTraceCol +=
      bary.x * getNorm(tris[i*3  ])+
      bary.y * getNorm(tris[i*3+1])+
      bary.z * getNorm(tris[i*3+2]);

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
void slabBox(unsigned int boxID, ray cr){
  
  theBvhHit = BVH_ESCAPE;
  
  vec3 toBoxOrigin = (getBB_Origin(boxID) - cr.o);
  
  // Transform to basis first. Test against axis planes.
  mat3 base = getBB_Basis(boxID);
  vec3 tro;
  vec3 trd;
  //dot the ray dir with the basis;
  trd = basis_transform_point(cr.d, base);
  float dR_i = trd.x;
  float dR_j = trd.y;
  float dR_k = trd.z;

  //dot the ray origin with the basis;
  tro = basis_transform_point(toBoxOrigin, base);
  float dRor_i = tro.x;
  float dRor_j = tro.y;
  float dRor_k = tro.z;

  //basis lengths should be prepared earlier.
  vec3 dims = getBB_Dims(boxID);

//Intersection points
  float mint_i, maxt_i, mint_j, maxt_j, mint_k, maxt_k;

  if (dR_i != 0){
  mint_i = dRor_i / dR_i;
  maxt_i = mint_i + dims[0]/dR_i;
}
if (dR_j != 0){
  mint_j = dRor_j / dR_j;
  maxt_j = mint_j + dims[1]/dR_j;
}
if (dR_k != 0){
  mint_k = dRor_k / dR_k;
  maxt_k = mint_k + dims[2]/dR_k;
}

  float mint = -FAR;
  float maxt =  FAR;

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
if (dR_i<0){ //
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
//
  theBvhHit.hits = (maxt>=mint);
  theBvhHit.t = mint;

  /*/ Debugging
  if (theBvhHit.hits){
   theBvhHit.hp = cr.o + theBvhHit.t *cr.d;
   theBvhHit.uv = theBvhHit.hp.xy;
  }*///
}

//TODO:test
////////////////////////////////////////
// Box Contains Point ///////////////////
bool boxcontains(unsigned int boxID, vec3 p){
  vec3 dims = getBB_Dims(boxID);
  vec3 relp = p - getBB_Origin(boxID);

// Todo: Investigate whether constructing individual floats
// and testing them to break early is faster
  vec3 tp = basis_transform_point(relp, getBB_Basis(boxID));

  return(
    0 < tp.x && tp.x < dims.x &&
    0 < tp.y && tp.y < dims.y &&
    0 < tp.z && tp.z < dims.z
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

  theTri.hits = (drFaceAB > 0 && drFaceBC> 0 && drFaceCA > 0
              || drFaceAB < 0 && drFaceBC< 0 && drFaceCA < 0); // hit backface
 // ); // cull backface
  theTri.hits = (drFaceAB == 1 || drFaceBC == 1 || drFaceCA == 1 || dot(mid,r.d) < 0) ?
  false : theTri.hits;

  theTri.elID = index;
  // Plane intersection
  theTri.n = normalize( cross( b-a, c-a ));
  theTri.t = dot(theTri.n,(a -r.o))/dot(theTri.n,r.d);
  theTri.hp = r.o + r.d*theTri.t;

}

//TODO:test
   /////////////////////////////////////////////////////
   // Trace Function   /////////////////////////////////
   /////////////////////////////////////////////////////
void leafTrace( ray r, unsigned int head ) {
   
   leafRay.bestHit = ESCAPE;
  
   //leafTrace writes to the global leafRay struct.

    unsigned int visit = head ; //Begin beyond the root

   //Make a stack of destinations 
   unsigned int stackCounter = 0;
   unsigned int visitStack[16]; // TODO resolve maximum stack depth and replace with a bitstack

   // PUSH TO ORIGIN PHASE ////////////////////////////////
    while( boxcontains (visit,r.o) ){
      //triangle case
      if (topo[visit-1] == -1)  {break;}
      //parent node case
      visitStack[stackCounter] = topo[visit*2];
      stackCounter ++;
      visit = topo[visit*2-1];
    }
  
    slabBox(visit,r);
    if (theBvhHit.hits) {
      visitStack[stackCounter] = visit;
      stackCounter += 1;
      leafRay.boxAccum += 0.002;
    }
    
   // INTERSECTION PHASE ////////////////////////////////////
    for (int  STEP = 0; STEP < MAX_TRAVERSAL_STEPS; STEP++){
      if (stackCounter<=0) break;
      visit = visitStack[-1+stackCounter--];

      slabBox(visit,r);
      if (!theBvhHit.hits) continue;
      if (theBvhHit.t > leafRay.bestHit.t){
        continue; 
      }
     
      leafRay.boxAccum += 0.03; //Visualise the bvh

      // Primitive case
      if (topo[visit*2-1] < 0){
      testTriIndex(r,topo[(visit)*2]);
        if( theTri.hits 
            && theTri.t < leafRay.bestHit.t //){
            && theTri.t > 0){
          leafRay.bestHit.hits=true;
          leafRay.bestHit = theTri;
        }
      continue;
      }
      unsigned int small = topo[visit * 2 - 1];
      unsigned int large = topo[visit * 2 ];//+ 1];
      visitStack[stackCounter++] = small;
      visitStack[stackCounter++] = large;
     }
} // Traversal escaped the tree. Got the best hit.
//////////////////////////////////////////////////////////
