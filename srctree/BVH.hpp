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
  glm::mat3 base;
  glm::vec3 origin;  //bounding box extends from 0 to 1 in the basis in all of its dimensions.
  float volume;

};
//Prims
struct bvhPrimitive{
  virtual ~bvhPrimitive() {}
  virtual BVH_BBox getBBox();
  int matID;
};


struct bvhTriangle : bvhPrimitive{
  glm::vec3 pa, pb, pc;   //Points
  glm::vec3 na, nb, nc;   //Normals
  int matID;
  BVH_BBox getBBox(); //TODO:get the longest side as x, project it to the adjacent for y, set z to 0, there you go.
};

//Nodes
struct BVHNode{
  BVH_BBox bb;
  virtual void updateBB(); 
  virtual ~BVHNode(){}
};//base type

struct BVHParentNode : BVHNode{
  BVHParentNode(BVHNode * a, BVHNode * b); //TODO:comparative constructor
  //optionally weight the tree, in a way that doesnt affect topology or emptiness maximization, but it may be useful to prefer checks against bigger volumes first.
  BVHNode * largest;
  BVHNode * smallest;
  void updateBB(); //TODO:check bounding boxes of children. Copy the bbox of the bigger one, and extend it to include both.
};

struct BVHLeaf : BVHNode{
  bvhPrimitive prim;
  void updateBB(); //TODO: get a bbox from the primitive
};

////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
///                               Main Host Side BVH Class                  ////
class BVH{

  public:
  void initBVH(); //sets up gl buffers
  std::vector<bvhPrimitive> leaves; //All the nitty gritty bits
  std::vector<BVHNode> forest;  // All the traversal start points.

  
  void clearScene(){
    forest.clear();
    leaves.clear();
  }

  //Primitive Loading:
  void addVerts(
      std::vector<glm::vec3> points, std::vector<glm::vec2> uvs);
  void addTris(
      std::vector<unsigned int> trispecs, int matID);

  //Building Functions
  private:
};
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//    HELPERS                           //
int sendBVH(BVH & theB, GLFWwindow * ctx);
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//IMPLEMENTATIONS


////////////////////////////////////////////////////////////////////////////////
