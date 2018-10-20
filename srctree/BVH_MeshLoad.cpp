
#include "BVH.hpp"
#include "cgra/mesh.hpp"

using namespace glm;
using namespace std;

void BVH::addData(vector<vec3> & in_vp, vector<vec3> & in_norm, vector<vec2> & in_uvs, 

    vector<unsigned int> & in_tris, mat4 & translation, mat4 & rotation){
  for (vec3 v : in_vp){
    vec4 v4p = vec4(
        v.x, v.y, v.z, 1.0
      );

/*    v4p = translation*rotation*v4p;
    vec3 v3t = vec3(v4p.x,v4p.y,v4p.z);


    v4p = translation*rotation*v4p;
    vec3 v3t = vec3(v4p.x,v4p.y,v4p.z);
    bvh_buffs.verts.push_back(v3t);*/ 
    bvh_buffs.verts.push_back(v); 
  }
 
  for (vec3 n : in_norm){

    vec4 n4p = vec4(
        n.x, n.y, n.z, 1.0
        );
/*
    n4p = rotation*n4p;
    vec3 n3t = vec3(n4p.x,n4p.y,n4p.z);
    bvh_buffs.normals.push_back(normalize(n3t));*/ 
    bvh_buffs.normals.push_back(normalize(n)); 
  }
 
  for (vec2 uv : in_uvs){
    bvh_buffs.uvs.push_back(uv); 
  }

  // Fill out the  primitive vectors
  for (unsigned int i :in_tris){
    bvh_buffs.trispecs.push_back(i); 
  }
 
}

void BVH::clearbuffs(){
  bvh_buffs.verts.clear();
  bvh_buffs.normals.clear();
  bvh_buffs.uvs.clear();
  bvh_buffs.trispecs.clear();
  bvh_buffs.matIDs.clear();

  bvh_buffs.bb_specs.clear();
  bvh_buffs.topo.clear();
}

void BVH::countNodes(){
  //check the leaf count
   lcount = leaves.size();
   pcount = parents.size();
   nodeCount = lcount+pcount;
}

BVHNode * BVH::fetchNode(unsigned int i){
 if (i>=lcount) return (BVHNode *) ( & (parents.at(i-lcount) ) );
  else           return (BVHNode *) ( & (leaves.at(i) ) );
  
  return nullptr;
}

void BVH::buildTopo(){

  clearScene();
  //Construct the trees

  //Put them in a forest

  // seems to work
  int forestCount = bvh_buffs.trispecs.size()/3;
  for (int i = 0; i < forestCount; i++){
    bvhTriangle nt;

    nt.pa = bvh_buffs.verts[ bvh_buffs.trispecs[i*3] ];
    nt.pb = bvh_buffs.verts[ bvh_buffs.trispecs[i*3+1] ];
    nt.pc = bvh_buffs.verts[ bvh_buffs.trispecs[i*3+2] ];

    nt.index = i;

    //add the leaf
    BVHLeaf newleaf= BVHLeaf(nt);
    newleaf.index = bb_counter;
    newleaf.bb.uniqueid = bb_counter++; 

    leaves.push_back(newleaf);
  }
    //^^think it works

  ////// Structure the forest
  ////
  countNodes();
  int headcount = lcount; //start with leaf amount of heads.
  vector<unsigned int> heads;
  for(int i=0; i< lcount; i++) heads.push_back(i);
  //the head array is initialised to match the leaves.

  //Rewrite !!!
  //
  //Match finding stage, mutual mingling
  while(headcount > 1){
    //  get the length of the head array
    for(int j = 0; j< headcount; j++) {

      BVHNode * thisOne = fetchNode(heads[j]);

      for(int i = 0; i< headcount; i++) {
        if ( j != i ){ //Avoid self clash

          BVHNode * thatOne = fetchNode(heads[j]);

        }
      }
    }

//////// Match making stage. //////////////////
///////
///////Matchmaker registers mutual matches.
// Unmatched nodes get pushed to the next round.

//Only go through once.
    unsigned int nextheadC = 0;
    vector<unsigned int> next_heads;
#define NUPARADD lcount+parents.size()-1

    for (int i=0 ; i< headcount; i++){

      cout << "Headcount :" << headcount << " heads \n";
      int A = i;
      BVHNode * bvA = fetchNode(heads[A]);
      int B = bvA -> want_headIndex;        //A wants B.
      if (B>=0 && (!(bvA->marked))){// Already marked, or -1, do not want check
        BVHNode * bvB = fetchNode(heads[B]);

        // Are they mutually matched?
        if(bvB->want_headIndex == A){  
          //Make a parent
          BVHParentNode nuPar = BVHParentNode( bvA, bvB );
          parents.push_back(nuPar);
          next_heads.push_back(NUPARADD);
          bvA->marked = true;
          bvB->marked = true;
        }
        else{
          //No match yet, push it to the next round.
          next_heads.push_back(heads[A]);
        }
      }//end Marked, -1 dont want check
    }
    heads = next_heads;
    headcount = heads.size();
  } // Down to one head.

}
