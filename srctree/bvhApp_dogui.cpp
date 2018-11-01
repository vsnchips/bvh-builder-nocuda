#include "includes-l1.hpp"

#include "bvhApp.hpp"

#include "imgui.h"

//#include <filesystem>

namespace fs = std::experimental::filesystem;

using namespace std;

void BVHApp_Application::doGUI() {

  ImGui::Begin("View");
  ImGui::SliderFloat3("Translate",&m_translation[0],-50.0f,50.0f, "%.5f",1.5f);
  ImGui::SliderFloat("Scale",&m_scale,0,5.0f, "%.5f", 2.5f);
  (ImGui::SliderFloat2("Rotate",&u_PolarLook[0],-M_PI,M_PI, "%.5f", 1.0f));

  ImGui::End();

  ImGui::Begin("Shader Controls");
  if (ImGui::Button("Reload Shader")){
    reloadShader();
  }
  ImGui::End();

  ImGui::Begin("Rendering Controls");
  ImGui::SliderInt("Head Node",&u_headNode,theBVH.nodeCount-100, theBVH.nodeCount-1);
  if(ImGui::Button("Reset the Head Pointer")){
    theBVH.countNodes();
    u_headNode = theBVH.nodeCount;
  }
  ImGui::Checkbox("Show BVH Primitives", &u_showPrims);
  ImGui::Checkbox("Highlight Triangle Edges", &u_primEdges);
  ImGui::Checkbox("Highlight BVH edges", &u_bvhEdges);
  ImGui::SliderInt("# of Triangles to include in Brute Force",&u_bruteForceCount,0, 2000);
  ImGui::Checkbox("Use Brute Force Ray Testing", &u_bruteForcePrims);
  ImGui::End();

  ImGui::Begin("BVH Build Controls");
  ImGui::SliderInt("Maximum Primitives",&maxPrims,0,5000);
  if(ImGui::Button( "Send the BVH to the GPU" )){
    sendBVH( theBVH, m_window );
  }
  if(ImGui::Button("Build the BVH")){
    theBVH.buildTopo(maxPrims);
    theBVH.countNodes();
    u_headNode = theBVH.nodeCount -1 ;
  }
  ImGui::End();

  ImGui::Begin("Mesh Loading");
  if(ImGui::Button("Load a model from a file")){
        nfdchar_t * objFile;
        NFD_OpenDialog(".obj,*","", &objFile);
      //theBVH.clearbuffs();
        loadObj((const char *)objFile, app_testmesh1);
        mesh2BVH(app_testmesh1);
  }

  if(ImGui::Button("Load a sphere")){
    //theBVH.clearbuffs();
      loadObj("../srctree/res/models/sphere.obj",app_testmesh1);
      gl_errorFlush("sphere");
      mesh2BVH(app_testmesh1);
  }

  if(ImGui::Button("Load a bunny")){
    //theBVH.clearbuffs();
    maxPrims = 0;
      loadObj("../models/darkorabbit.obj",app_testmesh1);
      gl_errorFlush("bunny");
      mesh2BVH(app_testmesh1);
  }
  if(ImGui::Button("Load a curtain")){
    //theBVH.clearbuffs();
    maxPrims = 0;
      loadObj("../models/curtain.obj",app_testmesh1);
      gl_errorFlush("bunny");
      mesh2BVH(app_testmesh1);
  }
  if(ImGui::Button("Load a mounted lion head")){
    //theBVH.clearbuffs();
    maxPrims = 0;
      loadObj("../models/lionshield.obj",app_testmesh1);
      gl_errorFlush("bunny");
      mesh2BVH(app_testmesh1);
  }

  if(ImGui::Button("Load a Pillar")){
    //theBVH.clearbuffs();
    maxPrims = 0;
      loadObj("../models/pillar.obj",app_testmesh1);
      gl_errorFlush("bunny");
      mesh2BVH(app_testmesh1);
  }

  if(ImGui::Button("Load a Scene")){
    theBVH.clearbuffs();
    maxPrims = 0;
      loadObj("../models/bunny.obj",app_testmesh1);
      mesh2BVH(app_testmesh1);
      loadObj("../models/lionshield.obj",app_testmesh1);
      mesh2BVH(app_testmesh1);
      loadObj("../models/curtain.obj",app_testmesh1);
      mesh2BVH(app_testmesh1);
      loadObj("../models/pillar.obj",app_testmesh1);
      mesh2BVH(app_testmesh1);
  }
    
  ImGui::End();

  ImGui::Begin("Messages");
  long double tick = (long double)(frame_timer)/(long double)CLOCKS_PER_SEC;
  ImGui::Text("Draw time: %Lf, \n Frame Rate : %Lf", tick, 1/tick); 
  ImGui::End();
}
