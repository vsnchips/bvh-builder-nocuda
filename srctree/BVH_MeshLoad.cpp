
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
void BVH::createLeaves(){

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
  }//End Buffer Filling

}

void BVH::buildTopo(){

  clearScene();
  //Get the raw leaves
  createLeaves();

  //Put them in a forest
  ////// Structure the forest /////
  countNodes();
  int headcount = lcount; //start with leaf amount of heads.
  vector<unsigned int> heads;
  //the head array is initialised to match the leaves.
  for(int i=0; i< lcount; i++) heads.push_back(i);


  //Rewrite !!!
  
  
  while(headcount > 1){ 
  //Plurality condition
  cout << "Headcount :" << headcount << " heads \n";

  //Match finding stage, mingling //////////////////
    for(int j = 0; j< headcount; j++) {

      BVHNode * thisOne = fetchNode(heads[j]); //Activate Picker
      // Forget about the last one!
      thisOne->want = nullptr;
      thisOne->wantbox.volume=-1;

      for(int i = 0; i< headcount; i++) {
        if ( j != i ){ //Avoid self clash

          BVHNode * thatOne = fetchNode(heads[i]); //Get Candidate
          // Try the match.

          // Recieve a pitch
          if(thatOne->want == thisOne){
              //accept conditions
              if(thisOne->wantbox.volume==-1 ||
                thatOne->wantbox.volume <
                thisOne->wantbox.volume) { 
              thisOne->want = thatOne;
              thisOne->wantbox = thatOne->wantbox;
           }
          }
          else { // No pitch, try a pitch
            BVH_BBox tryBoxA = BVH_BBox(&(thisOne->bb),&(thatOne->bb));
            BVH_BBox tryBoxB = BVH_BBox(&(thatOne->bb),&(thisOne->bb));

            //which one is better?
            BVH_BBox betterBox = 
              (tryBoxA.volume<tryBoxB.volume) ? tryBoxA : tryBoxB;

            if (betterBox.volume < thisOne->wantbox.volume ||
                thisOne->wantbox.volume == -1) 
              //Make the pitch.
            { thisOne->wantbox = betterBox;
              thisOne->want = thatOne; 
            }
          }//End new pitch case
        }//selfclash check
      }//inner loop
    }// End mingling!

    //////// Match making stage. //////////////////
    ///////
    ///////Matchmaker registers mutual matches.
    // Unmatched nodes get pushed to the next round.

    //Only go through once.
    unsigned int nextheadC = 0;
    vector<unsigned int> next_heads; next_heads.clear();
    #define NUPARADD lcount+parents.size()-1

    for (int i = 0 ; i < headcount; i++){

      int A = i;
      BVHNode * bvA = fetchNode(heads[A]);
      int B = bvA -> want_headIndex;        //A wants B.

      // Already marked
      if (bvA->marked) continue;
      //Every node wants a node. No nullptrs.
        BVHNode * bvB = bvA-> want;

        // Are they mutually matched?
        //if(bvB->want_headIndex == A){  
        if(bvB->want == bvA){  
          //Make a parent
          BVHParentNode nuPar = BVHParentNode( bvA, bvB );
          parents.push_back(nuPar);
          next_heads.push_back(NUPARADD);
          bvA->marked = true;
          bvB->marked = true;
        }
        //Neither marked, nor matched.
        //push to next round.
        else next_heads.push_back(heads[A]);
    }
    
    //End match making pass.
    
    heads = next_heads;
    headcount = heads.size();

  } // Down to one head.
}
