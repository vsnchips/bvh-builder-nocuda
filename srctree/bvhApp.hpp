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

   std::vector<std::string> app_shaderFilenames;
    vmpwStringStreamConcat * shaderStream;

    const char * fragShaderPath;

    bool clickon;
    bool mouseDown = false;
    bool sceneon = 1;

    GLuint quad_vao = 0;
    GLuint quad_vbo = 0;
    GLuint quad_ibo = 0;
   
    void gl_initQuad();
    void gl_drawQuad();

    int pickID;
   
    int pickKF;

    int lx = 2,ly = 2,lz = 2;
    float pickDepth=1;
    GLint triangleMode;

    int pickState = -1;

    GLFWwindow * dummy;
    BVHApp_renderer bvhRenderer  = BVHApp_renderer(dummy);
    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;

    bool kf_window_see = true;

    // Graphical Assets
    
    drawList app_stylePack;
    
    /////////////////////////////////////////// Part 1 Vars
    // The window object managed by GLFW
    GLFWwindow * m_window;

    GLFWwindow *  keyframe_window;

    // The shader program used for drawing
    cgra::Program m_program;
    // The mesh data
    cgra::Mesh m_mesh;
    cgra::Mesh m_spheremesh;

    // The current size of the viewport
    glm::vec2 m_viewportSize;
    
    // The current mouse position
    glm::vec2 m_mousePosition;

    // The translation of the mesh as a vec3
    glm::vec3 m_translation;
    // The scale of the mesh
    float m_scale;

    // A 4x4 matrix representing the rotation of the
    // mesh,
    // And a vec3 with the z/y/x/ coords.

    glm::vec3 xax,yax,zax;
    glm::vec3 polarrotation;
    glm::mat4 m_rotationMatrix,m_modelTransform;

    // Whether or not the left, middle or right buttons are down.
    bool m_mouseButtonDown[3];

    BVHApp_Application(GLFWwindow *win)
        : m_window(win),
          m_viewportSize(1, 1), m_mousePosition(0, 0),
          m_translation(0), m_scale(1), m_rotationMatrix(1) {
        m_mouseButtonDown[0] = false;
        m_mouseButtonDown[1] = false;
        m_mouseButtonDown[2] = false;

        bvhRenderer = BVHApp_renderer(m_window);
    }

    void setWindowSize(int width, int height) {
        m_viewportSize.x = float(width);
        m_viewportSize.y = float(height);


    }

    
    void init(const char *);
    void reloadShader();
    void reloadShader(const char *);

    void createCube();
    void loadObj(const char *filename,cgra::Mesh &);

    void freshEditBuff();
    void mainWindowPick();

    drawList testDrawList(cgra::Mesh * ctlMesh);
    void gl_ViewPrep();
    void updateScene(clock_t nt);

    void doGUI();

    void onKey(int key, int scancode, int action, int mods);

    void onMouseButton(int button, int action, int mods);

    void onCursorPos(double xpos, double ypos);

    void onScroll(double xoffset, double yoffset);

   
    void makeCurve();
    void styleCurve();
    void kfwin_oncursor(double xpos, double ypos);
    void kfwin_onMouse(int button, int action, int mods);
    void kfwin_onkey(int key, int scancode, int action, int mods);
    void kfwin_onscroll(double xoffset, double yoffset);

};

void gl_errorFlush(const char  * at);

