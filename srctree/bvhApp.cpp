#include "bvhApp.hpp"

using namespace std;
using namespace glm;

char * openShader(){
  nfdchar_t * fromFile;
  NFD_OpenDialog("*,glsl,frag","",&fromFile);
  return fromFile;
}
void BVHApp_Application::gl_initQuad(){

  glDeleteVertexArrays(1,&quad_vao);
  glDeleteBuffers(1,&quad_vbo);
  glDeleteBuffers(1,&quad_ibo);
  glGenVertexArrays(1,&quad_vao);
  glBindVertexArray(quad_vao);
  glGenBuffers(1,&quad_vbo);
  glBindBuffer(GL_ARRAY_BUFFER, quad_vbo);
  glGenBuffers(1,&quad_ibo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,quad_ibo);

  vector<vec3> points; points.clear();
  points.push_back(vec3(-1,-1,0));
  points.push_back(vec3(1,-1,0));
  points.push_back(vec3(1,1,0));
  points.push_back(vec3(-1,1,0));
  vector<unsigned int> indices; indices.clear();
  indices.push_back(0);
  indices.push_back(1);
  indices.push_back(2);
  indices.push_back(2);
  indices.push_back(3);
  indices.push_back(0);
 
  size_t size = sizeof(vec3) * 4;
  const void * buffHead =  reinterpret_cast<const void *>(&(points[0]));
  glBufferData(GL_ARRAY_BUFFER, size, buffHead, GL_STATIC_DRAW );


  size = sizeof(unsigned int) * 6;
  buffHead = reinterpret_cast<const void *> (&indices[0]);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, buffHead, GL_STATIC_DRAW); 

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(vec3),0);
  

}

void BVHApp_Application::gl_drawQuad(){
  //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  gl_errorFlush("polygonmode");
  glBindVertexArray(quad_vao);
  gl_errorFlush("bindvao");
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
  gl_errorFlush("drawElements");

}

void BVHApp_Application::reloadShader(){
     reloadShader(fragShaderPath);
}

void BVHApp_Application::reloadShader(const char * fragShaderFile){

   //Base app stuff 
    cgra::Program m_program;
    m_program = m_program.load_program(
    CGRA_SRCDIR "/res/shaders/simple.vs.glsl",
    fragShaderFile);
    //CGRA_SRCDIR "/res/shaders/simple.fs.glsl");
    
    /*if (!(m_program.m_islinked)){   //Broken!! Load the Default!
      printf("BROKEN LINK! Defaulting.\n");
      m_program = m_program.load_program(
      CGRA_SRCDIR "/res/shaders/simple.vs.glsl",
      CGRA_SRCDIR "/res/shaders/simple.fs.glsl");
      }
      else{
      printf("Linked Successfully.\n");
      }
         */
  gl_errorFlush("linking");
  bvhRenderer.pickProg = m_program;
   m_program.use();
  gl_errorFlush("use");
}

void BVHApp_Application::init(const char * fragShaderFile) {
    
  fragShaderPath = fragShaderFile;

   // Create a view matrix that positions the camera
    // 10 units behind the object
    viewMatrix = glm::mat4 (1);
    viewMatrix[3] = glm::vec4(0, 0, -1, 1);

    xax = glm::vec3(1.,0.,0.);
    yax = glm::vec3(0.,1.,0.);
    zax = glm::vec3(0.,0.,1.);

    m_translation.z=-2.0f;

    //Load the objs;
    loadObj("res/models/sphere.obj",m_spheremesh);
  gl_errorFlush("sphere");

    loadObj("res/models/bunny.obj",m_mesh);
  gl_errorFlush("bunny");
  
  triangleMode = GL_TRIANGLES;

//BVH App stuff
//
//
    if (!fragShaderFile) fragShaderFile = openShader();
    app_shaderFilenames.clear();
    app_shaderFilenames.push_back(string(fragShaderFile));
    shaderStream = new vmpwStringStreamConcat(&app_shaderFilenames);

    reloadShader();
    gl_initQuad();
   
}


void BVHApp_Application::loadObj(const char *filename,cgra::Mesh &targetMesh) {
    cgra::Wavefront obj;
    // Wrap the loading in a try..catch block
    try {
        obj = cgra::Wavefront::load(filename);
    } catch (std::exception e) {
        std::cerr << "Couldn't load file: '" << e.what() << "'" << std::endl;
        return;
    }


    // Replace these with appropriate values
    int numVertices  = obj.m_positions.size();
    int numTriangles = obj.m_faces.size();
    printf("\nVerts %u, Tris %u,\n", numVertices, numTriangles);
    printf("\n Vertex 0 : %lf %lf %lf\n", obj.position(10).x, obj.position(10).y, obj.position(10).z);

    cgra::Matrix<double> vertices(numVertices, 3);
    cgra::Matrix<unsigned int> triangles(numTriangles, 3);


    std::cout << "\nFilling Verts\n" << std::endl;

        // Add each position to the vertices matrix
        for(int i = 1; i <= numVertices;i++){
            vertices.setRow(i-1,{obj.m_positions[i-1].x,obj.position(i).y,obj.position(i).z});
        }

    std::cout << "\nFilling Tris\n" << std::endl;
        // Add each triangle's indices to the triangles matrix
        // Remember that Wavefront files use indices that start at 1
        for(int i = 0; i < numTriangles;i++) triangles.setRow(i,{obj.m_faces[i].m_vertices[0].m_p-1,
                                                                 obj.m_faces[i].m_vertices[1].m_p-1,
                                                                 obj.m_faces[i].m_vertices[2].m_p-1});
    targetMesh.maxdist = obj.range;
    targetMesh.setData(vertices, triangles);

}


void BVHApp_Application::freshEditBuff(){

      int width, height;
      glfwGetFramebufferSize(m_window, &width, &height);
      glViewport(0, 0, width, height);
      setWindowSize(width, height);
      glClearColor(0, 0, 0.1, 1); // Clears the color to a dark blue
      glClearDepth(1); // Clears the depth buffer to it's maximum value
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      glEnable( GL_DEPTH_TEST );

}

void BVHApp_Application::gl_ViewPrep(){

  //View Prep
    //Projection Setup
    float aspectRatio = m_viewportSize.x / m_viewportSize.y;
    // Calculate the projection matrix with a field-of-view of 45 degrees
    projectionMatrix = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 100.0f);

    // View Transforms
    m_modelTransform = glm::mat4(1.0f);
    viewMatrix = glm::mat4(1);
    viewMatrix[3] = glm::vec4(0, 0, -1, 1);

    viewMatrix *= glm::translate(glm::mat4(),m_translation);
    viewMatrix *= glm::scale(m_modelTransform,glm::vec3(m_scale));
    m_rotationMatrix = glm::mat4(glm::vec4(xax,0),glm::vec4(yax,0),glm::vec4(zax,0),glm::vec4(0.f,0.f,0.f,1.f));
    viewMatrix *= m_rotationMatrix;

}

void BVHApp_Application::updateScene(clock_t nt) {
  
  if(shaderStream->update(nt)){ 
    printf("Change Detected! \n");
    fragShaderPath = shaderStream->filenames->at(0).c_str(); 
    reloadShader(); 
    } 
  //Refresh viewport 
  glfwMakeContextCurrent(m_window);
  freshEditBuff();

  gl_ViewPrep();
    
  bvhRenderer.pickProg.setProjectionMatrix(projectionMatrix);
  bvhRenderer.pickProg.setViewMatrix(viewMatrix);
  m_program.setProjectionMatrix(projectionMatrix);
  m_program.setViewMatrix(viewMatrix);
    
  m_program.use();
  //bvhRenderer.execute();
  gl_drawQuad(); 
  
  glUseProgram(0);
               
  ImGui_ImplGlfwGL3_NewFrame();
  doGUI();
  ImGui::Render();

  glfwSwapBuffers(m_window);


  freshEditBuff();// reset the Viewport transform;

}
