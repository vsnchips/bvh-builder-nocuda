#include "BVH.cpp"

BVHNode::pick_partner(vector<BVHNode> & forest){

  want = &(forest[0]);
  float volume;

  BVH_BBox my_BB =  

    for (BVHNode candidate : forest ){
    }

}

void BVH_BBox::updateBasis(){
  basis[0] = points[1] - points[0]; //x axis
  basis[1] = points[3] - points[0]; //y axis
  basis[0] = points[4] - points[0]; //z axis
}

//////////////////////////////////////////////
//Comparative Constructor//////////////////////
//////////////////////////////////////////////

BVH_BBox::BVH_BBox(BVH_BBox * a, BVH_BBox * b){
  a->updateBasis();
  b->updateBasis();
  //consume b in a
  float volacb;//result
  vector<vec3> prop_AhasB = containing_set(a->points, b->points);
  vector<vec3> prop_BhasA = containing_set(b->points, a->points);
  //which set is smaller?
  float vol_a_has_b = setVolume( prop_AhasB );
  float vol_b_has_a = setVolume( prop_AhasB );

  points = vol_a_has_b < vol_b_has_a ? prop_AhasB : prop_BhasA;   //
  volume = min(vol_a_has_b,vol_b_has_a);
}


//////////////////////////////////////////////////////////////////
//Helper functions/////////////////////////////////////////////////
float setVolume(vector<vec3> set){
  float x = length(set[1]-set[0]);
  float y = length(set[3]-set[0]);
  float z = length(set[4]-set[0]);
  return x*y*z;
}

vector<vec3> containing_set(vec3 (&base)[8], vec3 (&mounter)[8], vec3* target){

  vector<vec3> new_points; new_points.clear();

  //make an array of relative points
  vec3 ib_rp[8];  //Relative;
  vec3 ib_rp_new[8];  //Relative temp;
  for(int i = 0; i < 8 < i++){ //Convert the relative points to the basis of A, with point 0 as the origin.
    vec3 relp = b->points[i] - a->points[0];
    ib_rp[i].x = dot( relp , normalize(a->base[0])) / length(a->base[0]);
    ib_rp[i].y = dot( relp , normalize(a->base[1])) / length(a->base[1]);
    ib_rp[i].z = dot( relp , normalize(a->base[2])) / length(a->base[2]);
  }

  //make a containing set in the basis

  //Convert the relative points to the basis of A.
  //Point 000
  for(int i=0; i < 8; i++){
    ib_rp_new[0] = vec3(
        min( ib_rp[i].x, 0 ), 
        min( ib_rp[i].y, 0 ), 
        min( ib_rp[i].z, 0 ) 
        );
  }
  new_points.push_back(ib_rp_new[0]);
  //Point 100
  for(int i=0; i < 8; i++){
    ib_rp_new[1] = vec3(
        max( ib_rp[i].x, 1 ), 
        min( ib_rp[i].y, 0 ), 
        min( ib_rp[i].z, 0 ) 
        );
  }
  new_points.push_back(ib_rp_new[1]);
  //Point 110
  for(int i=0; i < 8; i++){
    ib_rp_new[2] = vec3(
        max( ib_rp[i].x, 1 ), 
        max( ib_rp[i].y, 1 ), 
        min( ib_rp[i].z, 0 ) 
        );
  }
  new_points.push_back(ib_rp_new[2]);
  //Point 010
  for(int i=0; i < 8; i++){
    ib_rp_new[3] = vec3(
        min( ib_rp[i].x, 0 ), 
        max( ib_rp[i].y, 1 ), 
        min( ib_rp[i].z, 0 ) 
        );
  }
  new_points.push_back(ib_rp_new[3]);
  //Point 001
  for(int i=0; i < 8; i++){
    ib_rp_new[4] = vec3(
        min( ib_rp[i].x, 0 ), 
        min( ib_rp[i].y, 0 ), 
        max( ib_rp[i].z, 1 ) 
        );
  }
  new_points.push_back(ib_rp_new[4]);
  //Point 101
  for(int i=0; i < 8; i++){
    ib_rp_new[5] = vec3(
        max( ib_rp[i].x, 1 ), 
        min( ib_rp[i].y, 0 ), 
        max( ib_rp[i].z, 1 ) 
        );
  }
  new_points.push_back(ib_rp_new[5]);
  //Point 111
  for(int i=0; i < 8; i++){
    ib_rp_new[6] = vec3(
        max( ib_rp[i].x, 1 ), 
        max( ib_rp[i].y, 1 ), 
        max( ib_rp[i].z, 1 ) 
        );
  }
  new_points.push_back(ib_rp_new[6]);
  //Point 011
  for(int i=0; i < 8; i++){
    ib_rp_new[7] = vec3(
        min( ib_rp[i].x, 0 ), 
        max( ib_rp[i].y, 1 ), 
        max( ib_rp[i].z, 1 ) 
        );
  }
  new_points.push_back(ib_rp_new[7]);

  //the relative containing set is in ib_rp_new.
  //Convert them back using A's basis

  for (int i=0: i< 8 ; i++){
    new_points.push_back(
        a->points[0] + // go to base origin
        a-> base[0] * ib_rp_new[i].x +
        a-> base[1] * ib_rp_new[i].y +
        a-> base[2] * ib_rp_new[i].z +
        );
  }

  return new_points;

}

