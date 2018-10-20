#pragma once

// BVH HEADER FILE
// Has the structs and classes for the elements of the  bvh structure, 
// the BVH state, builder,  and converter class, 
// and the sender prototype.

#include "includes-l1.hpp"

struct bvh_ray{
  glm::vec3 o;
  glm::vec3 p;
  glm::vec3 r;
};

//Shared structs
struct BVH_BBox{
  glm::mat3 base;    //orientation
  std::vector<glm::vec3> points[8];
  glm::vec3 origin;  //bounding box extends from 0 to 1 in the basis in all of its dimensions.
  float volume;
  BVH_BBox(BVH_BBox * a, BVH_BBox * b);
};

//Prims
struct bvhPrimitive{
  virtual ~bvhPrimitive() {}
  virtual BVH_BBox getBBox();
  int matID;
  int index;
};


struct bvhTriangle : bvhPrimitive{
  glm::vec3 pa, pb, pc;   //Points
  glm::vec3 na, nb, nc;   //Normals
  int matID;
  BVH_BBox getBBox(); //TODO:get the longest side as x, project it to the adjacent for y, set z to 0, there you go.
};

//Nodes
struct BVHNode{
  int m_id;
  BVH_BBox bb;
  virtual void updateBB(); 
  BVHNode();
  virtual ~BVHNode(){}
  virtual void toBuffs();

  BVHNode * want = nullptr;
  bool marked = false;

};//base type

struct BVHParentNode : BVHNode{
  //optionally weight the tree, in a way that doesnt affect topology or emptiness maximization, but it may be useful to prefer checks against bigger volumes first.
  BVHNode largest;
  BVHNode smallest;
  void updateBB(); //TODO:check bounding boxes of children. Copy the bbox of the bigger one, and extend it to include both.
  BVHParentNode(BVHNode * a, BVHNode * b); //TODO:comparative constructor
};

struct BVHLeaf : BVHNode{
  bvhPrimitive prim;
  void updateBB(); //TODO: get a bbox from the primitive
  BVHLeaf(bvhPrimitive t) : prim(t){}
};

struct bufferPack{
  std::vector<glm::vec3> verts;
  std::vector<glm::vec3> normals;
  std::vector<glm::vec2> uvs;
  std::vector<int> tris;
  std::vector<int> matIDs;

  std::vector<glm::vec3> bb_specs;
  std::vector<glm::vec3> topo;

  unsigned int headIndex;
};

////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
///                               Main Host Side BVH Class                  ////

class BVH{

  public:
  void initBVH(); //sets up gl buffers
  std::vector<BVHLeaf> leaves;
  std::vector<BVHNode> forest;

  //basic content
  bufferPack bvh_buffs;
  void clearbuffs();
  
  void clearScene(){
    forest.clear();
    leaves.clear();
  }

  //The BVH Class can manage its gl buffers and send stuff to them:
  GLuint vertexbuff;
  GLuint normalbuff;
  GLuint uvbuff;
  GLuint tribuff;
  GLuint topobuff;
  GLuint bb_buff;

  //Primitive Loading:
  void addVerts(
      std::vector<glm::vec3> points, std::vector<glm::vec2> uvs);
  void addTris(
      std::vector<unsigned int> trispecs, int matID);

  //Building Functions
  private:
  void buildTopo();
};

////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//    HELPERS                           //
int sendBVH(BVH & theBVH, GLFWwindow * ctx);

float setVolume(std::vector<vec3> set);
std::vector<glm::vec3> containing_set( std::vector<glm::vec3> & base, std::vector<glm::vec3> & target);

////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//IMPLEMENTATIONS


////////////////////////////////////////////////////////////////////////////////
