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

//Shared classs
class BVH_BBox{
  public:
    glm::mat3 basis;    //orientation
    void updateBasis();
    std::vector<glm::vec3> points;
    float volume = -1;

    int isPrim = -1;
    unsigned int uniqueid;
    
    BVH_BBox(){}
    BVH_BBox(BVH_BBox * a, BVH_BBox * b);
};

//Prims
class bvhPrimitive{
  public:
    virtual BVH_BBox getBBox(){ printf("bvhPrimitive base class' getBB called"); abort();}
    int matID;
    int index;
    virtual ~bvhPrimitive(){}
};

class bvhTriangle : public bvhPrimitive{
  public:
    glm::vec3 pa, pb, pc;   //Points
    glm::vec3 na, nb, nc;   //Normals
    int matID;
    BVH_BBox getBBox(); //get the longest side as x, project it to the adjacent for y, set z to 0, there you go.
    ~bvhTriangle(){}
};


struct bufferPack{
  std::vector<glm::vec3> verts;
  std::vector<glm::vec3> normals;
  std::vector<glm::vec2> uvs;
  std::vector<unsigned int> trispecs;
  std::vector<int> matIDs;

  std::vector<glm::vec3> bb_specs;
  std::vector<int> topo;

  unsigned int headIndex;
};

//Nodes
class BVHNode{
  public:
    int index;

    BVH_BBox bb; //the box it has - redundant for primitive nodes
    BVH_BBox wantbox; //the Box it wants to be a part of
//    void pick_partner(std::vector<BVHNode> & forest); //pick a candidate from the forest

    int want_headIndex = -1; //temp ptr valid during a pass
    BVHNode * want = nullptr;
    bool marked = false;
    bool sighted = false;
  
    int type = -1;

    int parcount = 0; // should never be more than one!!

    virtual int structure(int p_id){std::cout << "Called base class structure method\n"; return 0;}
      //abort();}
    virtual int toBuffs(bufferPack * bp){std::cout << "base class doesnt have a toBuffs\n"; abort(); return 0;}
    virtual ~BVHNode(){}

};//base type

class BVHParentNode : public BVHNode{
  public:
    //optionally weight the tree, in a way that doesnt affect topology or emptiness maximization, but it may be useful to prefer checks against bigger volumes first.
    BVHNode *largest;
    BVHNode *smallest;
    //void updateBB(); //TODO:check bounding boxes of children. Copy the bbox of the bigger one, and extend it to include both.
//    int toBuffs(bufferPack * bp);
    int parent_toBuffs(bufferPack * bp);
//    int structure(int p_id);
    int par_structure(int p_id);
    BVHParentNode(BVHNode * a, BVHNode * b); //TODO:comparative constructor
    ~BVHParentNode(){}
};

class BVHLeaf : public BVHNode{
  public:
    bvhPrimitive prim;
    // void updateBB(); //TODO: get a bbox from the primitive
//    int toBuffs(bufferPack * bp);
    int leaf_toBuffs(bufferPack * bp);
//    int structure(int p_id);
    int leaf_structure(int p_id);
    
    BVHLeaf(bvhTriangle & t);
    ~BVHLeaf(){}
};

////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
///                               Main Host Side BVH Class                  ////

class BVH{

  public:
    void initBVH(); //sets up gl buffers
    std::vector<BVHLeaf> leaves;
    std::vector<BVHParentNode> parents;
    std::vector<BVHNode *> forest_ptrs;
    std::vector<unsigned int> forestHeads; // This gets refreshed on each pass.

    //basic content
    bufferPack bvh_buffs;
    void clearbuffs();

    void clearScene(){
      parents.clear();
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

    void addData(std::vector<glm::vec3> & in_vp, 
        std::vector<glm::vec3> & in_norm, std::vector<glm::vec2> & in_uvs, 
        std::vector<unsigned int> & in_tris, glm::mat4 & t, glm::mat4 & r);
    void addVerts(
        std::vector<glm::vec3> points, std::vector<glm::vec2> uvs);
    void addTris(
        std::vector<unsigned int> trispecs, int matID);

    //Building Functions
    unsigned int bb_counter=0; //bounding box count
    void buildTopo();
    void createLeaves();
    
    unsigned int nodeCount;
    unsigned int lcount;
    unsigned int pcount;
    void countNodes();
    BVHNode * fetchNode(unsigned int i);
};

////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//    HELPERS                           //
int sendBVH(BVH & theBVH, GLFWwindow * ctx);

float setVolume(std::vector<glm::vec3> & set);
std::vector<glm::vec3> containing_set( BVH_BBox * a, BVH_BBox * b);

////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//IMPLEMENTATIONS


////////////////////////////////////////////////////////////////////////////////
