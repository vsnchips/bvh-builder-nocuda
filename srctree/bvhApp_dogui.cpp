#include "includes-l1.hpp"

#include "bvhApp.hpp"

#include "imgui.h"

#include <filesystem>

namespace fs = std::filesystem;

using namespace std;

void BVHApp_Application::doGUI() {

  ImGui::Begin("View");
  ImGui::SliderFloat3("Translate",&m_translation[0],-50.0f,50.0f, "%.5f",1.5f);
  ImGui::SliderFloat("Scale",&m_scale,0,5.0f, "%.5f", 2.5f);
  if(ImGui::SliderFloat3("Rotate",&polarrotation[0],-M_PI,M_PI, "%.5f", 1.0f)){
    // User's spun the globe
    // Find the resulting matrix!

    //1. Transform the Z/north pole, X/west, and Y/celestial vectors via the input lat/long TODO: How do i get this into a mat3x3?
    if ( polarrotation.x == 0 ){
      zax = glm::vec3(0,0,1);
      yax = glm::vec3(0,1,0);
      xax = glm::vec3(1,0,0);
    }
    else {
      //fix for negative latitudes:
      float polx,poly;
      if (polarrotation.x < 0){
        polx =-polarrotation.x;
        poly = polarrotation.y + M_PI;
      } else {
        polx = polarrotation.x;
        poly = polarrotation.y;
      }

      zax = glm::rotate(
          (glm::rotate(glm::vec3(0.,0.,1.),polx,glm::vec3(0.,1.,0.))) // tilt it on Y over to X to latitude
          ,poly, glm::vec3(0.,0.,1.));  // spin it on true Zorth to longtitude

      //2.Find the normal and angle between Zorth and the new Z, and apply the same rotation to Xwest and YCelestial
      glm::vec3 tnorm = glm::cross(glm::vec3(0.,0.,1.),zax);
      yax = glm::rotate(glm::vec3(0.,1.,0.),polx,tnorm);
      xax = glm::rotate(glm::vec3(1.,0.,0.),polx,tnorm);
    }
    //3. Rotate X and Y around the tilted Z pole/
    yax = glm::rotate(yax, polarrotation.z, zax);
    xax = glm::rotate(xax, polarrotation.z, zax);

  };

  ImGui::End();

  ImGui::Begin("Shader Controls");

  if (ImGui::Button("Reload Shader")){
    reloadShader();
  }
  ImGui::End();

  ImGui::Begin("Draw Uniforms");
  ImGui::End();

  ImGui::Begin("BVH Controls");
  if(ImGui::Button( "Send to GPU" )){
    sendBVH( theBVH, m_window );
  }


  if(ImGui::Button("Build the BVH Topo")){
    theBVH.buildTopo();
  }


  if(ImGui::Button("Load a sphere")){
    theBVH.clearbuffs();
      loadObj("../srctree/res/models/sphere.obj",app_testmesh1);
      gl_errorFlush("sphere");

      mesh2BVH(app_testmesh1);
  }

  if(ImGui::Button("Load a bunny rabbit")){
    theBVH.clearbuffs();
      loadObj("../srctree/res/models/bunny.obj",app_testmesh1);
      gl_errorFlush("bunny");

      mesh2BVH(app_testmesh1);
  }

  if(ImGui::Button("Load a model from a file")){
        nfdchar_t * objFile;
        NFD_OpenDialog(".obj,*","", &objFile);

    theBVH.clearbuffs();
            loadObj((const char *)objFile, app_testmesh1);
        mesh2BVH(app_testmesh1);
  }

  ImGui::SliderInt("Maximum Primitives",&maxPrims,0,2000);

  ImGui::End();

  ImGui::Begin("Messages");
  long double tick = (long double)(frame_timer)/(long double)CLOCKS_PER_SEC;
  ImGui::Text("Draw time: %Lf, \n Frame Rate : %Lf", tick, 1/tick); 
  ImGui::End();
}
