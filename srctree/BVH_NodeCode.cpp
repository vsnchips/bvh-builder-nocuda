#include "BVH.hpp"

using namespace glm;
using namespace std;

BVHParentNode::BVHParentNode(BVHNode * a, BVHNode * b){

  if( a->wantbox.uniqueid !=b-> wantbox.uniqueid) {
    cout << " bounding box mismatch in parent constructor\n";
    abort();
  }

  bb = a->wantbox;

  if( a->bb.volume > b->bb.volume ) {
    largest = a;
    smallest = b;
  }
  else{
    largest = b;
    smallest = a;
  }
  
}

int BVHParentNode::structure(int p_id){
  //Depth first traversal  
  int sid = smallest->structure(p_id);
  int bid = largest->structure(  sid);
  index = bid + 1;                        //Head last;
  return index;

}

int BVHLeaf::structure(int p_id){
  //Depth first traversal  
  index = p_id + 1;                        //Head last;
  return index;
}

int BVHParentNode::toBuffs(bufferPack & bp){

  //Depth first traversal  
  smallest->toBuffs(bp); 
  largest->toBuffs(bp);

  //post
  bp.topo.push_back(smallest->index);
  bp.topo.push_back( largest->index);

  bp.bb_specs.push_back( bb.origin  ); 
  bp.bb_specs.push_back( bb.basis[0] ); 
  bp.bb_specs.push_back( bb.basis[1] ); 
  bp.bb_specs.push_back( bb.basis[2] ); 

  return (bp.topo.size()/2);

} 

int BVHLeaf::toBuffs(bufferPack & bp){
  bp.topo.push_back(-1);
  bp.topo.push_back( prim.index );
  return bp.topo.size()/2;
} 


