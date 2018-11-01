#include "bvhApp_renderer.hpp"
#include "bvhApp.hpp"

using namespace glm;

void BVHApp_renderer::loadPickShader(){
  pickProg = pickProg.load_program(
        CGRA_SRCDIR "/res/shaders/simple.vs.glsl",
        //CGRA_SRCDIR "/res/shaders/simple.fs.glsl"
        //CGRA_SRCDIR "/res/shaders/simple.fs.glsl"
        CGRA_SRCDIR "/res/shaders/bvhfrag.glsl"
           );
  gl_errorFlush(" load renderer picker shader");
}

void BVHApp_renderer::passUniforms(cgra::Program * p, uniforms * unfms){
  //ints
  for (auto const& v : unfms->i1){
    glUniform1i(glGetUniformLocation(p->m_glprogram,v.first.c_str()),v.second);
  }
  //floats
  for (auto const& v : unfms->f1){
    glUniform1f(glGetUniformLocation(p->m_glprogram,v.first.c_str()),v.second);
  }
  //vec3s
  for (auto const& v : unfms->f3){
    glUniform3fv(glGetUniformLocation(p->m_glprogram,v.first.c_str()),1,&v.second[0]);
  }
  //mat4s
  for (auto const& v : unfms->m4){
    glUniformMatrix4fv(glGetUniformLocation(p->m_glprogram,v.first.c_str()),1, GL_FALSE,&v.second[0][0]);
  }
}


void BVHApp_renderer::pickDraw(std::vector<drawStyle> & t){

pickProg.use();

// Clear the back buffer
glClearColor(255, 255, 255, 1);
glClearDepth(1);
glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

for(drawStyle d: t){
      passUniforms(&pickProg,&d.unfms);
      if (d.m_mesh && d.tag == "pickable") d.m_mesh->draw();
    }

}

void BVHApp_renderer::execute(std::vector<drawStyle> & target){

  // Pick Preview
 
 if (previewPick){
    pickDraw(target);
  }

  //The default overriden case
  else {
    for (drawStyle d : target){
      if (d.prog){
        if (c_prog != d.prog){
          d.prog->use();
          c_prog = d.prog;
        }
      }

      //uniform overrides
      if(d.unfms.i1["id"] > -1 && d.unfms.i1["id"] == highLight)
      {
        d.unfms.f3["ucol"] = vec3(0.8,0.8,0);
      }

      passUniforms(c_prog,&d.unfms);

      if (d.m_mesh){
        if (d.mode > -1) d.m_mesh->draw(d.mode);
        else d.m_mesh->draw();
      }

      if (d.tag == "lines"){
      if (d.vao){
       glBindVertexArray(d.vao);
       glLineWidth(2.0f);
       glDrawArrays(GL_LINE_STRIP, 0, d.primCount); 
      }
      }
    }
  }

}

int BVHApp_renderer::pickTest( std::vector<drawStyle> target, glm::vec2 & pickPos){

  checkSize();

int pickedID = -1;

pickDraw(target);

// Reading in after drawing
unsigned char pixel[4];
glReadPixels(pickPos.x,
rtHei - pickPos.y, 1, 1,   GL_RGBA,   GL_UNSIGNED_BYTE,   &pixel);

float pickDepth; //might need the depth to find things;

glReadPixels(pickPos.x,
rtHei - pickPos.y, 1, 1,   GL_DEPTH_BUFFER_BIT,   GL_FLOAT,   &pickDepth);

if (!(pixel[0]==255) || !(pixel[1]==255) || !(pixel[2]==255) ){

    pickedID = pixel[0] + pixel[1]*256 + pixel[2]*256*256;

    printf("RED %i\n", pixel[0]);
    printf("GREEN %i\n", pixel[1]);
    printf("BLUE %i\n", pixel[2]);

}

printf("Picked id %d\n", pickedID );
printf("Picked  color %d, %d, %d, %d at %f, %f \n", pixel[0], pixel[1], pixel[2], pixel[3],
    float(pickPos.x), m_viewportSize.y - pickPos.y);
printf("Picked pickDepth %f\n", pickDepth );
return pickedID;

}
