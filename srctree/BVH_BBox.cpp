#include "BVH.hpp"

using namespace std;
using namespace glm;

//////////////////////////////////////////////
//Comparative Constructor//////////////////////
//////////////////////////////////////////////


BVH_BBox::BVH_BBox(BVH_BBox * a, BVH_BBox * b){ //Constructs a new bounding box that minimally encloses two child boxes.

  //consume b in a
  float volacb;//result
  vector<vec3> prop_AhasB = containing_set(a, b);
  vector<vec3> prop_BhasA = containing_set(b, a);
  //which set is smaller?
  float vol_a_has_b = setVolume( prop_AhasB );
  float vol_b_has_a = setVolume( prop_AhasB );

  points = vol_a_has_b < vol_b_has_a ? prop_AhasB : prop_BhasA;   //
  updateBasis();
  volume = glm::min(vol_a_has_b,vol_b_has_a);
  isPrim = 0;
}

//////////////////////////////////////////////
/////// Triangle Constructor /////////////////
//////////////////////////////////////////////


//////////////////////////////////////////////
/////// Functionality ///////////////////////
//////////////////////////////////////////////
void BVHNode::pick_partner(vector<BVHNode> & forest){

  want = &(forest[0]);
  BVH_BBox wantbox( &bb , &(want-> bb) );
  float volume = wantbox.volume;

    for ( BVHNode candidate : forest ){
      BVH_BBox getBox( &bb , &candidate.bb );
      if (getBox.volume < wantbox.volume){
          wantbox = getBox;
          want = &candidate;
      }
    }
}

void BVH_BBox::updateBasis(){

  basis[0] = points[1] - points[0]; //x axis
  basis[1] = points[3] - points[0]; //y axis
  basis[2] = points[4] - points[0]; //z axis
}


//////////////////////////////////////////////////////////////////
//Helper functions/////////////////////////////////////////////////
float setVolume(vector<vec3> set){
  float x = length(set[1]-set[0]);
  float y = length(set[3]-set[0]);
  float z = length(set[4]-set[0]);
  return x*y*z;
}

vector<vec3> containing_set(BVH_BBox * a, BVH_BBox * b){

  vector<vec3> new_points; new_points.clear();

  //make an array of relative points
  vec3 ib_rp[8];  //Relative;
  vec3 ib_rp_new[8];  //Relative temp;
  for(int i = 0; i < 8; i++){ //Convert the relative points to the basis of A, with point 0 as the origin.
    vec3 relp = b->points[i] - a->points[0];
    ib_rp[i].x = dot( relp , normalize(a->basis[0])) / length(a->basis[0]);
    ib_rp[i].y = dot( relp , normalize(a->basis[1])) / length(a->basis[1]);
    ib_rp[i].z = dot( relp , normalize(a->basis[2])) / length(a->basis[2]);
  }

  //make a containing set in the basis

  //Convert the relative points to the basis of A.
  //Point 000
  for(int i=0; i < 8; i++){
    ib_rp_new[0] = vec3(
        glm::min( ib_rp[i].x, 0.f ), 
        glm::min( ib_rp[i].y, 0.f ), 
        glm::min( ib_rp[i].z, 0.f ) 
        );
  }
  //Point 100
  for(int i=0; i < 8; i++){
    ib_rp_new[1] = vec3(
        glm::max( ib_rp[i].x, 1.f ), 
        glm::min( ib_rp[i].y, 0.f ), 
        glm::min( ib_rp[i].z, 0.f ) 
        );
  }
  //Point 110
  for(int i=0; i < 8; i++){
    ib_rp_new[2] = vec3(
        glm::max( ib_rp[i].x, 1.f ), 
        glm::max( ib_rp[i].y, 1.f ), 
        glm::min( ib_rp[i].z, 0.f ) 
        );
  }
  //Point 010
  for(int i=0; i < 8; i++){
    ib_rp_new[3] = vec3(
        glm::min( ib_rp[i].x, 0.f ), 
        glm::max( ib_rp[i].y, 1.f ), 
        glm::min( ib_rp[i].z, 0.f ) 
        );
  }
  //Point 001
  for(int i=0; i < 8; i++){
    ib_rp_new[4] = vec3(
        glm::min( ib_rp[i].x, 0.f ), 
        glm::min( ib_rp[i].y, 0.f ), 
        glm::max( ib_rp[i].z, 1.f ) 
        );
  }
  //Point 101
  for(int i=0; i < 8; i++){
    ib_rp_new[5] = vec3(
        glm::max( ib_rp[i].x, 1.f ), 
        glm::min( ib_rp[i].y, 0.f ), 
        glm::max( ib_rp[i].z, 1.f ) 
        );
  }
  //Point 111
  for(int i=0; i < 8; i++){
    ib_rp_new[6] = vec3(
        glm::max( ib_rp[i].x, 1.f ), 
        glm::max( ib_rp[i].y, 1.f ), 
        glm::max( ib_rp[i].z, 1.f ) 
        );
  }
  //Point 011
  for(int i=0; i < 8; i++){
    ib_rp_new[7] = vec3(
        glm::min( ib_rp[i].x, 0.f ), 
        glm::max( ib_rp[i].y, 1.f ), 
        glm::max( ib_rp[i].z, 1.f ) 
        );
  }

  //the relative containing set is in ib_rp_new.
  //Convert them back using A's basis

  for (int i = 0; i < 8; i++){
    new_points.push_back(
        a-> points[0] + // go to basis origin
        a-> basis[0] * ib_rp_new[i].x +
        a-> basis[1] * ib_rp_new[i].y +
        a-> basis[2] * ib_rp_new[i].z 
        );
  }

  return new_points;

}

