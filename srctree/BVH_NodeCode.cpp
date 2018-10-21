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

  type=1; 
  
}

BVHLeaf::BVHLeaf(bvhTriangle & t){   //TODO: sort outhe the polymorphism here
      bb = t.getBBox(); 
      type=0; 
}

int BVHParentNode::structure(int p_id){
  //Depth first traversal
  //leaf cases
  BVHLeaf * leaf_s = dynamic_cast<BVHLeaf *>( smallest );
  BVHLeaf * leaf_l = dynamic_cast<BVHLeaf *>( largest );
  BVHParentNode * par_s = dynamic_cast<BVHParentNode *>( smallest );
  BVHParentNode * par_l = dynamic_cast<BVHParentNode *>( largest );

  int sid,bid;

  if( leaf_s != nullptr )    sid = leaf_s->structure(p_id);
  else if( par_s != nullptr) sid = par_s->structure( p_id);
  else {cout << "small node cannot be downcast!\n";}
  if( leaf_l  != nullptr)    bid = leaf_l->structure( sid);
  else if (par_l != nullptr) bid = par_l->structure(  sid);
  else {cout << "large node cannot be downcast!\n";}
  
  cout << "structure a PARENT \n";
  index = bid + 1;                        //Head last;
  return index;

}

int BVHLeaf::structure(int p_id){
  //Depth first traversal  
  cout << "structure a LEAF \n";
  index = p_id + 1;                        //Head last;
  return index;
}

int BVHParentNode::toBuffs(bufferPack * bp){

  //Depth first traversal  
  BVHLeaf * leaf_s = dynamic_cast<BVHLeaf *>( smallest );
  BVHLeaf * leaf_l = dynamic_cast<BVHLeaf *>( largest );
  BVHParentNode * par_s = dynamic_cast<BVHParentNode *>( smallest );
  BVHParentNode * par_l = dynamic_cast<BVHParentNode *>( largest );

  if( leaf_s != nullptr )
  leaf_s->toBuffs(bp); 
  else if( par_s != nullptr) 
  par_s->toBuffs(bp); 
  else {cout << "small node cannot be downcast!\n";}
  if( leaf_l  != nullptr)    
  leaf_l->toBuffs(bp);
  else if (par_l != nullptr) 
  par_l->toBuffs(bp);
  else {cout << "large node cannot be downcast!\n";}
 
//  smallest->toBuffs(bp); 
// largest->toBuffs(bp);

  //post
  bp->topo.push_back(smallest->index);
  bp->topo.push_back( largest->index);

  bp->bb_specs.push_back( bb.origin  ); 
  bp->bb_specs.push_back( bb.basis[0] ); 
  bp->bb_specs.push_back( bb.basis[1] ); 
  bp->bb_specs.push_back( bb.basis[2] ); 

  return (bp->topo.size()/2);

} 

int BVHLeaf::toBuffs(bufferPack * bp){
  bp->topo.push_back(-1);
  bp->topo.push_back( prim.index );
  
  //Primitive's bb potentially irrelevant for tracing
  //but the data layout expects it to be there
  bp->bb_specs.push_back( bb.origin  ); 
  bp->bb_specs.push_back( bb.basis[0] ); 
  bp->bb_specs.push_back( bb.basis[1] ); 
  bp->bb_specs.push_back( bb.basis[2] ); 
  return bp->topo.size()/2;
} 


