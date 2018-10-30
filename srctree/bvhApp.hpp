#pragma once

#include "cgra/mesh.hpp"
#include "cgra/shader.hpp"

#include "glm/glm.hpp"
#include "bvhApp_renderer.hpp"

//src includes 
#include <algorithm>
#include <cmath>
#include <iostream>
#include <stdexcept>
#include <cstring>

#include "includes-l1.hpp"
#include "imgui.h"

#include "cgra/matrix.hpp"
#include "cgra/wavefront.hpp"
#include "math.h"


//bvh includes

#include "BVH.hpp"
#include "BVHRenderer.hpp"

typedef struct shareGlobals{
//fill this later

};

class BVHApp_Application {
public:

  //Organise this file into responsibilities:
  // It updates
    void updateScene(clock_t nt);

  // Has a BVH Renderer:
    BVHRenderer  * app_BVHRenderer;

 // Loads and has a shader:
    void reloadShader(const char *);
    void reloadShader();
    std::vector<std::string> app_shaderFilenames;
    std::vector<std::string> app_compshaderFilenames;
    vmpwStringStreamConcat * compShaderStream;
    vmpwStringStreamConcat * shaderStream;
    const char * fragShaderPath;
    const char * compShaderPath;
    // The shader programs
    cgra::Program m_program;
    GLuint compshader;

  // Has a BVH Maker/Maintainer
    BVH theBVH;
  // Sends bvhs to the gpu:
    void sendBuffs();

  // Loads and has graphical assets:
    void loadObj(const char *filename,cgra::Mesh &);
    void mesh2BVH(cgra::Mesh &m);
    void mesh2BVH(cgra::Mesh &m, glm::mat4 & translation, glm::mat4 & rotation);
    cgra::Mesh app_testmesh1;
    cgra::Mesh app_testmesh2;
     int maxPrims = 400;// Limit the amount of primitives to send to the BVH for testing
 
   //Scene Drawing
    void drawApp();
    void gl_ViewPrep();
  ////Maintains and sends scene data to the shader:
  //Transforms
    void sendUniforms();
    glm::mat4 viewMatrix; //Deprecated
    glm::mat4 projectionMatrix;
    float m_scale;
    glm::vec2 m_viewportSize;
    glm::vec3 m_translation;
    glm::vec3 xax,yax,zax;
    glm::vec3 polarrotation;
    glm::mat4 m_rotationMatrix,m_modelTransform;
  //Lights
    glm::vec3 lightDir;
  //Other uniforms
    
  //Has and operates a window
    GLFWwindow * m_window;
    GLFWwindow * dummy;
    void freshEditBuff();
    void setWindowSize(int width, int height) {
        m_viewportSize.x = float(width);
        m_viewportSize.y = float(height);
    }
   
  //Has and operates a GUI
    void doGUI();

  //Maps interactions to stuff
    void onKey(int key, int scancode, int action, int mods);
    void onMouseButton(int button, int action, int mods);
    void onCursorPos(double xpos, double ypos);
    void onScroll(double xoffset, double yoffset);
    // Interaction methods
    void mainWindowPick();   //TODO:reimplement this when needed
    // The clock
    clock_t app_time;
    //User input
    bool clickon;
    bool mouseDown = false;
    bool sceneon = 1;
    int pickID;
    float pickDepth=1;
    int pickState = -1;
    glm::vec2 m_mousePosition;
    bool m_mouseButtonDown[3];
   
//Initialisation 
  //Default Constructor
    BVHApp_Application(GLFWwindow *win)
        : m_window(win),
          m_viewportSize(1, 1), m_mousePosition(0, 0),
          m_translation(0), m_scale(1), m_rotationMatrix(1) {
        m_mouseButtonDown[0] = false;
        m_mouseButtonDown[1] = false;
        m_mouseButtonDown[2] = false;

        app_BVHRenderer = new BVHRenderer(m_window); //TODO: Swap for BVHRenderer
    }
   
  //Post construction initialisation
    void init(const char *, const char *);

};


