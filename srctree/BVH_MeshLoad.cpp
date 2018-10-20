#include" BVH.hpp"
#include "cgra/mesh.hpp"

using namespace glm;
using namespace std;

int BVHParentNode::structure(int p_id){
  //Depth first traversal  
  int sid = smallest.structure(p_id);
  int bid = biggest.structure(  sid);
  m_id = bid + 1;                        //Head last;
  return m_id;

}

int BVHLeaf::structure(int p_id){
  //Depth first traversal  
  m_id = p_id + 1;                        //Head last;
  return m_id;
}

int BVHParentNode::toBuffs(bufferPack * bp){

  //Depth first traversal  
  smallest.toBuffs(bp); 
  biggest.toBuffs(bp);

  //post
  bp.topo.push_back(smallest.index);
  bp.topo.push_back( biggest.index);

  bp.bb_specs.push_back( bb.origin  ); 
  bp.bb_specs.push_back( bb.base[0] ); 
  bp.bb_specs.push_back( bb.base[1] ); 
  bp.bb_specs.push_back( bb.base[2] ); 

  return (bp.topo.size()/2);

} 

int BVHLeaf::toBuffs(bufferPack * bp){
  bp.topo.push_back(-1);
  bp.topo.push_back( prim.index );
  return bp.topo.size()/2;
} 


void addMesh(cgra::Mesh inMesh, mat4 translation, mat4 rotation){

  //get verts in an array
  vector<vec3> vs_in;
  for( Vertex v: inMesh.m_vertices ){
    vs_in.push_back(v.m_position);
  }

  //get normals in an array
  vector<vec3> ns_in;
  for( Vertex v: inMesh.m_vertices ){
    ns_in.push_back(v.m_normal);
  }

  //get normals in an array
  vector<vec3> uvs_in;
  for( Vertex v: inMesh.m_vertices ){
    uvs_in.push_back(v.m_normal);
  }


  for (vec3 v : vs_in){
    verts.push_back(translation*rotation*v); 
  }
  for (vec3 n : ns_in){
    normals.push_back(normalize(rotation*gn)); 
  }
  for (vec2 uv : uvs_in){
    uvs.push_back(uv); 
  }
}

void BVH::clearbuffs(){
  bvh_buffs.verts.clear();
  bvh_buffs.normals.clear();
  bvh_buffs.uvs.clear();
  bvh_buffs.tris.clear();
  bvh_buffs.matIDs.clear();

  bvh_buffs.bb_specs.clear();
  bvh_buffs.topo.clear();
}


void BVH::buildTopo(){

  //Construct the trees

  //Put them in a forest
  forest.clear();

  int forestCount = tris.size()/3;
  for (int i = 0; i < forestCount; i++){
    bvhPrimitive nt;

    nt.pa = verts[ tris[i*3]];
    nt.pa = verts[ tris[i*3+1]];
    nt.pa = verts[ tris[i*3+2]];

    nt.index = i;

    forest.push_back(BVHLeaf(nt));
  }

  // Structure the forest
  //
  vector<BVHNode>nextForest; nextForest.clear();
  while (forestCount > 0){

    // Trees pick one another and propose
    for(BVHNode tree : forest){}
    // Matching trees get:
    for(BVHNode tree : forest){
      // put into parent nodes
      // 
      // Parent node gets put into parent forest
      //
      //
    }

    forest = nextForest;
  }

  //now print the forest into a bb buffer and a topo buffer;
  BVHNode * head = &(forest[0]);
  head-> structure(-1);
  clearbuffs();
  head-> tobuffs(bvh_buffs);
  bvh_buffs.headIndex = head-> m_id;

}
