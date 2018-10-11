#pragma once

#include "includes-l1.hpp"

struct bvh_ray{
  glm::vec3 o;
  glm::vec3 p;
  glm::vec3 r;
};
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
}


struct BVH_BBox{
  glm::mat3 base;
  glm::vec3 origin;  //bounding box extends from 0 to 1 in the basis in all of its dimensions.
  float volume;

};

class BVHNode{
  BVH_BBox bb;
  virtual void updateBB();  
};

class BVHParentNode : BVHNode{
  
  BVHParentNode(BVHNode * a, BVNode * b); //TODO:comparative constructor
  //optionally weight the tree, in a way that doesnt affect topology or emptiness maximization, but it may be useful to prefer checks against bigger volumes first.
  BVHNode * largest;
  BVHNode * smallest;
  void updateBB(); //TODO:check bounding boxes of children. Copy the bbox of the bigger one, and extend it to include both.

};

class BVHLeaf : BVHNode{
  bvhPrimitive prim;
  void updateBB(); //TODO: get a bbox from the primitive
};

BVHNode BVH::proposeNode( BVHNode * a, BVHNode * b){
  BVHNode proposal;
  proposal = BVHNode(a,b);
  virtual ~BVHNode() {};
}

//////////// Main Host Side BVH Class/////////////////////////////////////////////////////////////////////////////
class BVH{

  public:
  void initBVH(); //sets up gl buffers
  std::vector<Primitive> leaves; //All the nitty gritty bits
  std::vector<BVHNode> forest;  // All the traversal start points.

  
  void clearScene(){
    theForestry.forest.clear();
    theForestry.leaves.clear();
  }

  void addTris(std::vector<vec3> points, std::vector<vec2> uvs, std::vector<unsigned int> trispecs, s, int matID);

  //Building Functions
  private:
  BVHNode proposeNode( BVHNode * a, BVHNode * b);
};

int sendBVH(BVH & theB, GLFWwindow * ctx);
