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
      type = 0; 
}

