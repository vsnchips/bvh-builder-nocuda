#include "bvhApp.hpp"

using namespace std;
using namespace glm;

void BVHApp_Application::updateScene(clock_t nt) {

// Process input states
//
  doWasd();

  u_PolarLook.y = (u_PolarLook.y < -glm::pi<float>()/2) ? -glm::pi<float>()/2: u_PolarLook.y;
  u_PolarLook.y = (u_PolarLook.y > glm::pi<float>()/2) ? glm::pi<float>()/2: u_PolarLook.y;
  
  m_rotationMatrix = rotate( mat4(1), u_PolarLook.y, vec3(1,0,0)); 
  m_rotationMatrix = rotate( m_rotationMatrix, u_PolarLook.x, vec3(0,1,0)); 

  //Check Changes to the comp shader 
  if(compShaderStream->update(nt)){ 
    printf("Change In comp shader detected! \n");
    compShaderPath = compShaderStream->filenames->at(0).c_str(); 
    reloadShader(); 

    //Run the comp shader, too
    glUseProgram(compshader);
    glDispatchCompute(1,1,1);

    glMemoryBarrier(GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT);
    //glDispatchCompute( GLuint buffer );
  } 


  //Check Changes to the frag shader 
  if(shaderStream->update(nt)){ 
    printf("Change Detected in frag shader! \n");
    fragShaderPath = shaderStream->filenames->at(0).c_str(); 
    reloadShader(); 
 
    //Run the comp shader, too
    glUseProgram(compshader);
    glDispatchCompute(1,1,1);
    glMemoryBarrier(GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT);
    //glDispatchCompute( GLuint buffer );

   } 


   //Update view stuff
  sendUniforms();
  drawApp();
}

char * openShader(){
  nfdchar_t * fromFile;
  NFD_OpenDialog("*,glsl,frag","",&fromFile);
  return fromFile;
}


void BVHApp_Application::reloadShader(){
     reloadShader(fragShaderPath);
}

//Shader compilation helper
void checkShader(GLuint cshad){
  // Check to see if the compilation was successful
        GLint isCompiled = 0;
        glGetShaderiv(cshad, GL_COMPILE_STATUS, &isCompiled);

        if (isCompiled == GL_FALSE) {
            // Get the length of the error message
            GLint maxLength = 0;
            glGetShaderiv(cshad, GL_INFO_LOG_LENGTH, &maxLength);

            //The maxLength includes the NULL character
            std::string infoLog(maxLength, 0);
            glGetShaderInfoLog(cshad, maxLength, &maxLength, &infoLog[0]);

            // Print the error mesage
            std::cerr << "Failed to compile shader:\n" << infoLog << std::endl;

            //We don't need the shader anymore.
            glDeleteShader(cshad);
        }

}
void BVHApp_Application::reloadShader(const char * fragShaderFile){

    //Compute Shader
  std::ifstream fs(compShaderPath);
  std::stringstream compSStr;
  compSStr << fs.rdbuf();
  std::string comp_src = compSStr.str();
  const char * ccstr = comp_src.c_str();

  GLuint cshader = glCreateShader(GL_COMPUTE_SHADER);
  glShaderSource(cshader, 1, &ccstr, nullptr);
  glCompileShader(cshader);
  checkShader(cshader);
  gl_errorFlush("compute shader compile");

  compshader = glCreateProgram();
  glAttachShader(compshader,cshader);
  glLinkProgram(compshader);
  gl_errorFlush("compute shader link");


    //Fragment Shader
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
  m_program.use();
  gl_errorFlush("use");
  app_BVHRenderer->bvh_comp_program = compshader;
  app_BVHRenderer->bvh_frag_program = m_program.m_glprogram;

  sendBVH(theBVH,m_window);

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
    maxPrims += numTriangles;

    targetMesh.maxdist = obj.range;
    targetMesh.setData(vertices, triangles);

}

void BVHApp_Application::mesh2BVH(cgra::Mesh & inMesh){
  mat4 id(1.0);
  mesh2BVH(inMesh, id, id);
}

void BVHApp_Application::mesh2BVH(cgra::Mesh & inMesh, mat4 & translation, mat4 & rotation){
 
  //get verts in an array
  vector<vec3> vs_in;vs_in.clear();
  for(int i=0; i< inMesh.m_vertices.size(); i++){
    cgra::Mesh::Vertex v = inMesh.m_vertices[i];
    vs_in.push_back(v.m_position);
  }

  //get normals in an array
  vector<vec3> ns_in;ns_in.clear();
  for(int i=0; i< inMesh.m_vertices.size(); i++){
    cgra::Mesh::Vertex v = inMesh.m_vertices[i];
    ns_in.push_back(v.m_normal);
  }

  //get uvs in an array
  vector<vec2> uvs_in;uvs_in.clear();
  for(int i=0; i< inMesh.m_vertices.size(); i++){
    uvs_in.push_back( vec2(1.0));  //no uvs on these meshes yet
  }

  vector<vec2> dummyUV;dummyUV.clear();

  theBVH.addData(vs_in, ns_in, dummyUV, inMesh.m_indices, translation, rotation, maxPrims);

}

void BVHApp_Application::drawApp(){
  //Refresh viewport 
  glfwMakeContextCurrent(m_window);
  freshEditBuff();
  gl_ViewPrep();
  m_program.setProjectionMatrix(projectionMatrix);
  m_program.setViewMatrix(viewMatrix);
  m_program.use();

  frame_timer=clock();
  app_BVHRenderer->execute();
  frame_timer = clock() - frame_timer;
  ImGui_ImplGlfwGL3_NewFrame();
  doGUI();
  ImGui::Render();

  glfwSwapBuffers(m_window);

  freshEditBuff();// reset the Viewport transform;




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
    viewMatrix[3] = glm::vec4(0, 0, 1, 1);

    viewMatrix *= glm::translate(glm::mat4(),m_translation);
    viewMatrix *= glm::scale(m_modelTransform,glm::vec3(m_scale));
    viewMatrix *= m_rotationMatrix;

}

void BVHApp_Application::sendUniforms(){
   glUniformMatrix4fv(glGetUniformLocation(app_BVHRenderer->bvh_frag_program,"viewRotation"),1, GL_FALSE, &m_rotationMatrix[0][0]);
   glUniform3fv(glGetUniformLocation(app_BVHRenderer->bvh_frag_program,"viewPosition"),1, &m_translation[0]);
  
   glUniform1i(glGetUniformLocation(app_BVHRenderer->bvh_frag_program,"u_previewBBs"), u_previewBBs); 
   glUniform1i(glGetUniformLocation(app_BVHRenderer->bvh_frag_program,"u_primEdges"), u_primEdges); 
   glUniform1i(glGetUniformLocation(app_BVHRenderer->bvh_frag_program,"u_bvhEdges"), u_bvhEdges); 
   glUniform1i(glGetUniformLocation(app_BVHRenderer->bvh_frag_program,"u_showPrims"), u_showPrims); 
   glUniform1i(glGetUniformLocation(app_BVHRenderer->bvh_frag_program,"u_bruteForcePrims"), u_bruteForcePrims); 
   
   glUniform1i(glGetUniformLocation(app_BVHRenderer->bvh_frag_program,"u_headNode"), u_headNode); 
   glUniform1i(glGetUniformLocation(app_BVHRenderer->bvh_frag_program,"u_bruteForceCount"), u_bruteForceCount); 
   
   glUniformMatrix4fv(glGetUniformLocation(app_BVHRenderer->bvh_frag_program,"u_sceneTransforms"),20, GL_FALSE, &u_sceneTransforms[0][0][0]);
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
// GUI implementation defined in seperate file
void BVHApp_Application::init(const char * compShaderFile, const char * fragShaderFile) {
   
    app_BVHRenderer = new BVHRenderer(m_window);

    //View Stuff
    viewMatrix = glm::mat4 (1);
    viewMatrix[3] = glm::vec4(0, 0, 5, 1);
    xax = glm::vec3(1.,0.,0.);
    yax = glm::vec3(0.,1.,0.);
    zax = glm::vec3(0.,0.,1.);
    m_translation.z=5.0f;

    //Load the objs;
    loadObj("../srctree/res/models/sphere.obj",app_testmesh1);
    gl_errorFlush("sphere");
    loadObj("../srctree/res/models/bunny.obj",app_testmesh2);
    gl_errorFlush("bunny");

    //Shader tracking stuff
    if (!fragShaderFile) {
      fragShaderFile = openShader();
    }
      fragShaderPath = fragShaderFile;

    if (!compShaderFile) {
      compShaderFile = openShader();
    }
      compShaderPath = compShaderFile;

    app_compshaderFilenames.clear();
    app_compshaderFilenames.push_back(string(compShaderFile));

    app_shaderFilenames.clear();
    app_shaderFilenames.push_back(string(fragShaderFile));
    
    compShaderStream = new vmpwStringStreamConcat(&app_compshaderFilenames);
    shaderStream = new vmpwStringStreamConcat(&app_shaderFilenames);
    reloadShader();
    
  //Renderer Stuff
    app_BVHRenderer->initQuad();
   
}



