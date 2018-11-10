#include "bvhApp.hpp"
// Input Handlers

using namespace std;
using namespace glm;

void BVHApp_Application::onMouseButton(int button, int action, int) {

    if (button >=0 && button < 3) {
      mouseDown = true; 
        // Set the 'down' state for the appropriate mouse button
        if (button ==0){
            if( action == GLFW_PRESS) {
              
   glm::vec2 glMousePos = glm::vec2(m_mousePosition.x, m_viewportSize.y - m_mousePosition.y);

//General Window Condition
//
  mainWindowPick();
  }
 
  //Got pick ID
  clickon = pickID > 0;
  printf("clickon %s\n" , clickon ? "true" : "false");

  if (clickon) {}
  else {
    clickon = false;
    mouseDown = false;
    printf("unclick\n");
  }
  }
  m_mouseButtonDown[button] = action == GLFW_PRESS;
  }
}

void BVHApp_Application::mainWindowPick(){
//TODO:reimplement this when needed 
}

void BVHApp_Application::onCursorPos(double xpos, double ypos) {


    // Make a vec2 with the current mouse position
    glm::vec2 currentMousePosition(xpos, ypos);

    // Get the difference from the previous mouse position
    glm::vec2 mousePositionDelta = currentMousePosition - m_mousePosition;
    mousePositionDelta *= 0.012;

    if (glm::length(mousePositionDelta)>0){


        static bool click=false;
        static glm::vec3 sxa, sya, sza;
        static glm::vec2 fclick;


       if (m_mouseButtonDown[GLFW_MOUSE_BUTTON_LEFT]) {   // Look around

          u_PolarLook +=  mousePositionDelta;

        }

     if (m_mouseButtonDown[GLFW_MOUSE_BUTTON_MIDDLE]) {  // Pan case
       vec4 panDir = vec4 ( -mousePositionDelta.x, mousePositionDelta.y, 0.f, 1.f);
       panDir = inverse( m_rotationMatrix ) * panDir;
       m_translation += vec3(panDir.x,panDir.y,panDir.z);
    }if (m_mouseButtonDown[GLFW_MOUSE_BUTTON_RIGHT]) {    // Roll forward
       vec4 panDir = vec4 ( mousePositionDelta.x,0,mousePositionDelta.y, 1);
       panDir = inverse( m_rotationMatrix )* panDir;
       m_translation += vec3(panDir.x,panDir.y,panDir.z);
    }
     // Update the mouse position to the current one
     m_mousePosition = currentMousePosition;
    }
}


void BVHApp_Application::onKey(int key, int scancode, int action, int mods) {

    if(key == GLFW_KEY_W){
    if (action == GLFW_PRESS) wasd[0] = true;
    if (action == GLFW_RELEASE) wasd[0] = false;
    }


    if(key == GLFW_KEY_A){
    if (action == GLFW_PRESS) wasd[1] = true;
    if (action == GLFW_RELEASE) wasd[1] = false;
    }


    if(key == GLFW_KEY_S){
    if (action == GLFW_PRESS) wasd[2] = true;
    if (action == GLFW_RELEASE) wasd[2] = false;
    }



    if(key == GLFW_KEY_D){
    if (action == GLFW_PRESS) wasd[3] = true;
    if (action == GLFW_RELEASE) wasd[3] = false;
    }

    // `(void)foo` suppresses unused variable warnings
    (void)key;
    (void)scancode;
    (void)action;
    (void)mods;
}

void BVHApp_Application::onScroll(double xoffset, double yoffset) {
    // `(void)foo` suppresses unused variable warnings
    (void)xoffset;
    (void)yoffset;
}

void BVHApp_Application::doWasd(){    // `(void)foo` suppresses unused variable warnings
  if (wasd[0])
    {
      glm::vec4 panDir = glm::vec4 ( 0.00,0.00,-0.05, 1.f);
       panDir = inverse(m_rotationMatrix) * panDir;
       m_translation += glm::vec3(panDir.x,panDir.y,panDir.z);
    }

  if (wasd[1])
    {
      glm::vec4 panDir = glm::vec4 ( -0.05,0.00,0.00, 1.f);
       panDir = inverse(m_rotationMatrix) * panDir;
       m_translation += glm::vec3(panDir.x,panDir.y,panDir.z);
    }

  if (wasd[2])
    {
      glm::vec4 panDir = glm::vec4 ( 0.00,0.00,0.05, 1.f);
       panDir = inverse(m_rotationMatrix) * panDir;
       m_translation += glm::vec3(panDir.x,panDir.y,panDir.z);
    }

  if (wasd[3])
    {
       vec4 panDir = glm::vec4( 0.05,0.00,0.00, 1.f);
       panDir = inverse(m_rotationMatrix) * panDir;
       m_translation += vec3(panDir.x,panDir.y,panDir.z);
    }
}



