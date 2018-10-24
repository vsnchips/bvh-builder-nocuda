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

int BVHParentNode::par_structure(int p_id){
  //Depth first traversal
  //leaf cases
  int sid,bid;

  BVHLeaf * sl = dynamic_cast<BVHLeaf *>(smallest);
  BVHLeaf * ll = dynamic_cast<BVHLeaf *>(largest);
  BVHParentNode * sp = dynamic_cast<BVHParentNode *>(smallest);
  BVHParentNode * lp = dynamic_cast<BVHParentNode *>(largest);

  if(smallest -> type ==1)  sid = sp->par_structure(p_id);
  else                      sid = sl->leaf_structure(p_id);
  if(largest -> type ==1)   bid = lp->par_structure(sid);
  else                      bid = ll->leaf_structure(sid);
  
  cout << "structure a PARENT \n";
  index = bid + 1;                        //Head last;
  return index;

}

int BVHLeaf::leaf_structure(int p_id){
  //Depth first traversal  
  cout << "structure a LEAF \n";
  index = p_id + 1;                        //Head last;
  return index;
}

int BVHParentNode::parent_toBuffs(bufferPack * bp){

  //Depth first traversal 
  
  BVHLeaf * sl = dynamic_cast<BVHLeaf *>(smallest);
  BVHLeaf * ll = dynamic_cast<BVHLeaf *>(largest);
  BVHParentNode * sp = dynamic_cast<BVHParentNode *>(smallest);
  BVHParentNode * lp = dynamic_cast<BVHParentNode *>(largest);
  if(smallest->type == 1)  sp->parent_toBuffs(bp); 
  else  sl->leaf_toBuffs(bp); 
  if(largest->type == 1)  lp->parent_toBuffs(bp); 
  else ll->leaf_toBuffs(bp); 

  //post
  bp->topo.push_back(smallest->index);
  bp->topo.push_back( largest->index);

  bp->bb_specs.push_back( bb.origin  ); 
  bp->bb_specs.push_back( bb.basis[0] ); 
  bp->bb_specs.push_back( bb.basis[1] ); 
  bp->bb_specs.push_back( bb.basis[2] ); 

  return (bp->topo.size()/2);

} 

int BVHLeaf::leaf_toBuffs(bufferPack * bp){
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


