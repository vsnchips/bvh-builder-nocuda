
#include "BVH.hpp"
#include "cgra/mesh.hpp"

using namespace glm;
using namespace std;

void breakF(){
  int breakline;
}

void BVH::addData(vector<vec3> & in_vp, vector<vec3> & in_norm, vector<vec2> & in_uvs, 

    vector<unsigned int> & in_tris, mat4 & translation, mat4 & rotation){
  for(int i = 0; i < in_tris.size(); i++){
    vec3 v = in_vp[i];
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
 
  for(int i = 0; i < in_norm.size(); i++){
    vec3 n = in_norm[i];

    vec4 n4p = vec4(
        n.x, n.y, n.z, 1.0
        );
/*
    n4p = rotation*n4p;
    vec3 n3t = vec3(n4p.x,n4p.y,n4p.z);
    bvh_buffs.normals.push_back(normalize(n3t));*/ 
    bvh_buffs.normals.push_back(normalize(n)); 
  }
 
  for(int i = 0; i < in_uvs.size(); i++){
    vec2 uv = in_uvs[i];
    bvh_buffs.uvs.push_back(uv); 
  }

  // Fill out the  primitive vectors
  for(int i = 0; i < in_tris.size(); i++){
    bvh_buffs.trispecs.push_back(in_tris[i]); 
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

  cout << "grabbing at index " << i << "\n\
    leaves size " << leaves.size() << "\n";

 if (i>=leaves.size()) return ( & (parents.at(i-leaves.size()) ) );
 return ( & (leaves.at(i) ) );
  
}
void BVH::createLeaves(){

  int forestCount = bvh_buffs.trispecs.size()/3;
  //int forestCount = 4;

  //We know the amount of leaves. Reserve their memory.
  leaves.reserve(forestCount);
  parents.reserve(forestCount-1);


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
    cout << "created leaf " << leaves[i].index << "\n";

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
 
  //Get Absolute bounding box:
  vec3 low;
  vec3 high;
  low=high=bvh_buffs.verts[0];
  for(vec3 p :bvh_buffs.verts){
    low=glm::min(p,low);
    high=glm::max(p,high);
  }
  float size = length(high-low);
  cout << "Bonding Box span" << size << "\n";
  cout << "lowx" << low.x << "\n";
  cout << "lowy" << low.y << "\n";
  cout << "lowz" << low.z << "\n";
  cout << "highx" << high.x << "\n";
  cout << "highy" << high.y << "\n";
  cout << "highz" << high.z << "\n";
  //Time for a dirty improvement of a dirty sorter without using 
  //parallelism:
  //Check neighbors within a certain distance
  
  while(headcount > 1){ 

    cout << "\n New head List!\n";
    for( int h : heads){
      cout << h << " ";
    }

    // GPU version: send updated head list to an SSBO
    // Then do a polling pass on the gpu: 
    
    //Plurality condition
  cout << "\nNEW PASS Headcount :" << headcount << " heads \n";

  //Match finding stage, mingling //////////////////
    for(int j = 0; j < headcount; j++) {

      BVHNode * thisOne = fetchNode(heads[j]); //Activate Picker
      // Forget about the last one!
      thisOne->want = nullptr;
      thisOne->wantbox.volume=-1;
      for(int i = 0; i< headcount; i++) {
        if ( j != i ){ //Avoid self clash

          BVHNode * thatOne = fetchNode(heads[i]); //Get Candidate
          // Try the match.
        if (thisOne -> index < 0 || thisOne -> index > 2*leaves.size()) breakF();

          // Recieve a pitch
          if(thatOne->want == thisOne){
              //accept conditions
              if(thisOne->wantbox.volume==-1 ||
                thatOne->wantbox.volume <
                thisOne->wantbox.volume) { 
              thisOne->want = thatOne;
    cout << " want ptr = want->index " << (thisOne->want == fetchNode(thisOne->want->index)) <<  "\n";
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
            {
if (thisOne->index == 10 ) breakF();
              thisOne->wantbox = betterBox;
              thisOne->want = thatOne; 
    cout << " want ptr = want->index " << (thisOne->want == fetchNode(thisOne->want->index)) <<  "\n";
            }
          }//End new pitch case
        }//selfclash check
      }//inner loop
    }// End mingling!

    //////// Match making stage. //////////////////
    ///////
    ///////Matchmaker registers mutual matches.
    // Unmatched nodes get pushed to the next round.

    //Gpu version: Pull the updated want list from the gpu with image load/store
    

    //Only go through once.
    vector<unsigned int> next_heads; next_heads.clear();
   
#define NUPARADD leaves.size()+parents.size()

    for (int i = 0 ; i < headcount; i++){

      BVHNode * bvA = fetchNode(heads[i]);

      cout << " Checking mark of"  << heads[i] << " indexed as " << bvA -> index << ": \n";
      cout << "bvA marked as " << bvA->marked << "\n";
      // If Already marked
      if ((bvA->marked)==true){
       continue;
      }

      //Gpu version: checks here to want should reference the want image
      //
        BVHNode * bvB = bvA-> want;
        //cout << " bvb want ptr = index " << (bvB == fetchNode(bvB->index)) <<  "\n";
       // if (bvB -> index < 0 || bvB -> index > 2*leaves.size()) breakF();
       //if (bvA -> index < 0 || bvA -> index > 2*leaves.size()) breakF();
        if(bvB->want == bvA){  
          //Make a parent
          
          // Debug print
          printf("Making new parent from:\n\
              node %d\n\
              node %d\n", bvA->index, bvB->index);

          BVHParentNode nuPar = BVHParentNode( bvA, bvB );

          //Index iteration
          nuPar.index = NUPARADD;

          bvA->parcount = bvA->parcount+1;
          bvB->parcount = bvB->parcount+1;

          if(bvA->parcount > 1) cout << "OOPS! Node " << bvA->index << " has " << bvA->parcount << " parents!\n";


          next_heads.push_back( NUPARADD );
        cout << " bvb want ptr = index " << (bvB == fetchNode(bvB->index)) <<  "\n";
          parents.push_back(nuPar);
        cout << " bvb want ptr = index " << (bvB == fetchNode(bvB->index)) <<  "\n";
          cout << "There are now this many parents: " << parents.size() <<"\n";
          
          bvA->marked = true;
          bvB->marked = true;

          BVHNode * checkN = fetchNode(bvB->index);
          //cout << " check N marked as : " << checkN->marked << "\n";
          //cout << " Equality of fetchNode bvb-index and bvb: " << (checkN==bvB) << "\n";
          //cout << " Equality of fetchNode bvb-index and bvb: " << (fetchNode(bvB-index)==bvB) << "\n";

          cout << "marked"<<  bvA -> index << " as TRUE \n";
          cout << "marked"<<  bvB -> index << " as TRUE \n";
        }
        
        //Neither marked, nor matched.
        //push to next round.
        else{ next_heads.push_back(heads[i]);
          cout << "No match, push this head to the next round. \n";
          cout << next_heads.size() << " Heads next round, at least\n";
        }
    }
    
    //End match Fmaking pass.
    
    heads = next_heads;
    headcount = heads.size();

  } // Down to one head.


// To fix:
//This is all fucking broken:
//But i need a way to print the tree.
// Each parent node stores its children as pointers.
// It might aswell simply store absolute uint fetchnode references to them.
// Use the fetchNode command to walk the tree and print the list.


vector<int> topo_list; topo_list.clear();

vector<vec3> bb_list; bb_list.clear();

for (BVHLeaf l : leaves){
  //topo
  topo_list.push_back(-1);
  topo_list.push_back(l.index);

  //bb
  bb_list.push_back(l.bb.origin);
  bb_list.push_back(l.bb.basis[0]);
  bb_list.push_back(l.bb.basis[1]);
  bb_list.push_back(l.bb.basis[2]);

}
//Concatenate the parents after the leaves
for (BVHParentNode p: parents){

  //topo
  topo_list.push_back(p.smallest->index);
  topo_list.push_back(p.largest->index);

  //bb
  bb_list.push_back(p.bb.origin);
  bb_list.push_back(p.bb.basis[0]);
  bb_list.push_back(p.bb.basis[1]);
  bb_list.push_back(p.bb.basis[2]);

}

//cout << "topo_list: " << topo_list << "\n";
//cout << "bb_list: " << bb_list << "\n";
breakF();

bvh_buffs.topo = topo_list;
bvh_buffs.bb_specs = bb_list;

cout << "Host BVH buffers updated!";

}


