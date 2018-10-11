#include "bvhApp.hpp"
// Input Handlers

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
  
   freshEditBuff();
   bvhRenderer.pickProg.setViewMatrix(viewMatrix);
   bvhRenderer.pickProg.setProjectionMatrix(projectionMatrix);
                pickID = bvhRenderer.pickTest(app_stylePack, m_mousePosition);
                bvhRenderer.highLight = pickID;
}

void BVHApp_Application::onCursorPos(double xpos, double ypos) {


    // Make a vec2 with the current mouse position
    glm::vec2 currentMousePosition(xpos, ypos);

    // Get the difference from the previous mouse position
    glm::vec2 mousePositionDelta = currentMousePosition - m_mousePosition;

    if (glm::length(mousePositionDelta)>0){


        static bool click=false;
        static glm::vec3 sxa, sya, sza;
        static glm::vec2 fclick;


       if (m_mouseButtonDown[GLFW_MOUSE_BUTTON_LEFT]) {

           static int width, height;
           glfwGetWindowSize(m_window, &width, &height);

           int size = glm::min(width,height);

        if (pickID == -1){

           click = true;

               fclick = glm::vec2(-1+2*m_mousePosition.x/width,-1+2*m_mousePosition.y/height);
               sxa = xax;
               sya = yax;
               sza = zax;

            glm::vec2 nowpos = glm::vec2(-1+2*currentMousePosition.x/width,-1+2*currentMousePosition.y/height);

            if (height>width) {nowpos.y *= height/width; fclick.y *= height/width;}
            else {nowpos.y *= width/height; fclick.y *= width/height;}

            nowpos.x *= 0.8;
            nowpos.y *=0.8;
            fclick.x *= 0.8;
            fclick.y *=0.8;

           //Transform the coords to match the target object
           //
           
            nowpos.x*=-m_translation.z;
            nowpos.y*=-m_translation.z;
            fclick.x*=-m_translation.z;
            fclick.y*=-m_translation.z;
            nowpos.x = nowpos.x - m_translation.x;
            fclick.x = fclick.x - m_translation.x;
            nowpos.y = nowpos.y + m_translation.y;
            fclick.y = fclick.y + m_translation.y;


            nowpos.x /=m_scale;
            nowpos.y /=m_scale;

            fclick.x /=m_scale;
            fclick.y /=m_scale;


            if (glm::length(fclick) > 1) fclick = glm::normalize(fclick);
            if (glm::length(nowpos) > 1) nowpos = glm::normalize(nowpos);

            //vectorise Arc Points
            printf("\ncurrentMouse: %lf, %lf",fclick.x,fclick.y);
            glm::vec4 apA = glm::vec4(fclick.x,-fclick.y,glm::cos(glm::asin(glm::min(glm::length(fclick),0.99999f))),1.);
            glm::vec4 apB = glm::vec4(nowpos.x,-nowpos.y,glm::cos(glm::asin(glm::min(glm::length(nowpos),0.99999f))),1.);


            glm::vec3 apA3 = glm::vec3(apA.x,apA.y,apA.z);
            glm::vec3 apB3 = glm::vec3(apB.x,apB.y,apB.z);
            float t = glm::acos(glm::dot(glm::normalize(apA3),glm::normalize(apB3)));

            if (isnan(t) ) {printf("  T NAN !!!");
                t = 0.f;
                apA=glm::vec4(1.);
                apB=glm::vec4(1.);
                apA3=glm::vec3(1.,0.,0);
                apB3=glm::vec3(0.,0.,1.);
            }

            glm::vec3 n = glm::cross(apA3,apB3);
            xax = glm::rotate(sxa,t,n);
            yax = glm::rotate(sya,t,n);
            zax = glm::rotate(sza,t,n);

            //Transform modified by arcball tnwiddling
            //This codes updates the polar coords:

            //1.Get Latitude and Longtitude
            polarrotation.x = glm::acos(glm::dot(glm::vec3(0.,0.,1.),zax)); // -pi<Latitude<=pi ;

            polarrotation.y = glm::atan(zax.y,zax.x);

            //2. Get the normal of current and reference Z basevecs
            glm::vec3 tiltnorm = glm::cross(zax,glm::vec3(0.,0.,1.));
            glm::vec3 uprightX = glm::rotate(xax,polarrotation.x,tiltnorm);
            glm::vec3 uprightY = glm::rotate(yax,polarrotation.x,tiltnorm);

            // Get the Z angle
            polarrotation.z = glm::acos(glm::dot(uprightX,glm::vec3(1.0f,0.f,0.f)));
            }  else {

              
                }
            }

        }



     if (m_mouseButtonDown[GLFW_MOUSE_BUTTON_MIDDLE]) {
         static int width, height;
         glfwGetWindowSize(m_window, &width, &height);
         int size = glm::max(width,height);
         glm::vec2 shift = glm::vec2((m_translation.z)*-mousePositionDelta.x/size,(m_translation.z)*mousePositionDelta.y/size);
         m_translation = glm::vec3(m_translation.x+shift.x,m_translation.y+shift.y,m_translation.z);


    }if (m_mouseButtonDown[GLFW_MOUSE_BUTTON_RIGHT]) {
         //
         m_scale += glm::max(m_scale,0.1f)*0.1*-mousePositionDelta.y;
    }
     // Update the mouse position to the current one
     m_mousePosition = currentMousePosition;
}


void BVHApp_Application::onKey(int key, int scancode, int action, int mods) {
    // `(void)foo` suppresses unused variable warnings

    if(key == GLFW_KEY_A && action == GLFW_PRESS) // play/pause
    {
    }

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


