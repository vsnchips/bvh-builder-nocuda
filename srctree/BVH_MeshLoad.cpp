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


void BVH::buildTopo(){

  //Construct the trees

  //Put them in a forest
  forest.clear();

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

    forest.push_back(newleaf);
    forest_ptrs.push_back( &(forest[forest.size()-1] ) );
  }

  // Structure the forest
  //
  
  forestCount = forest.size();
  while (forestCount > 1){
    cout << forestCount << " Trees in the forest \n";
    forestCount = 0;

    // Trees pick one another and propose
    for(BVHNode * thistree : forest_ptrs){
      thistree->sighted = true;

      //Found an unpaired tree!
      if(!(thistree->marked))
      forestCount ++;
      {

     for(int that = 0; that < forest.size(); that++){
    // for(BVHNode * thattree : forest_ptrs){
       BVHNode * thattree = forest_ptrs[that];

       if (thistree != thattree ) 
       {
                 //Brute force for low-ish  poly test
        if(thattree->want == thistree  &&  thattree->wantbox.volume>=0 && 
           ((thattree->wantbox.volume < thistree->wantbox.volume)
           || (thistree->wantbox.volume<0))
           )
        {
          // tentatively accept the proposal, pending a better one.
          //
          thistree->wantbox = thattree->wantbox;
          thistree->want = thattree;
        
        }
        else //No recieved proposal, test one.
        {
          //make a proposal!
          //
        BVH_BBox proposal = BVH_BBox(&(thistree->bb), &(thattree->bb));
        //Increment the counter for every proposed BB!
        proposal.uniqueid = bb_counter++; 
        if (proposal.volume < thistree->wantbox.volume || thistree->wantbox.volume<0){
          thistree->wantbox = proposal;
          thistree->want = thattree;
         }// end proposal assignment
        }// agree or make fresh proposal
       }// self ref check
      }//end checking those trees
      } // marked in earlier pass check
    } //end checking these trees
    //All the trees have picked their matches

   
    // Mark paired trees as children and add their parents
    for(BVHNode  *thistree : forest_ptrs){
      for(BVHNode  *thattree : forest_ptrs){
       if (thistree != thattree){ //Brute force for low-ish  poly test

         // Matching trees get:
         if(thistree->want == thattree && thattree->want == thistree
             &&
             !(thistree->marked) && !(thattree->marked))
         {
           thistree->marked = true;
           thattree->marked = true;
           // put into parent nodes
           BVHParentNode newParent = BVHParentNode( thistree, thattree );
           // Parent node gets put into parent forest
           forest.push_back(newParent);
           forest_ptrs.push_back( &(forest[forest.size()-1] ) );

          //Decrement the count
          forestCount--;

         }// do we match?
       }//self reference check
      }//end those trees
    }//end these trees

  }// end forest plurality
  // The forest has been structured.

  //now print the forest into a bb buffer and a topo buffer;
  BVHNode * head = &(forest[0]);
  head-> structure(-1);
  clearbuffs();
  head-> toBuffs(bvh_buffs);
  bvh_buffs.headIndex = head-> index;

}
