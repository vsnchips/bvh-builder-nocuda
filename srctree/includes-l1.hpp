#pragma once

//Base includes
//STD LIB ///////////////

#include <sys/stat.h>
#include <iostream>
#include <istream>
#include <ostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>

#include <chrono>
#include <time.h>

#include <math.h>

/////////////////////
#include "cgra/mesh.hpp"
#include "cgra/shader.hpp"

#include "glm/glm.hpp"

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/euler_angles.hpp"
#include "glm/gtx/rotate_vector.hpp"

#include "imgui.h"
#include "cgra/imgui_impl_glfw_gl3.h"

#include "submods/nfd.h"

#include "GL/glew.h"
#include "GLFW/glfw3.h"

//portable module includes

#include "submods/stringStreamConCat/vmpwStringStreamConcat.hpp"

//Debugging:
void gl_errorFlush(const char  * at);

