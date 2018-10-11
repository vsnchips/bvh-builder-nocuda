#pragma once

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "glm/glm.hpp"

namespace cgra {

    class Program {
        // The OpenGL object representing the program

        Program(GLuint prog) : m_glprogram(prog) { }
    public:
        Program() : m_glprogram(0) { }

        bool m_islinked = false;

        // Load the program from two files, a vertex shader and
        GLuint m_glprogram;
        // a fragment shader.
        Program load_program(const char *vertex_shader_file,
                                    const char *fragment_shader_file);

        // Tells OpenGL to use this shader program
        void use();

        // Sets the model matrix for the program.
        // This controls the position, orientation and
        // scale of the model.
        void setModelMatrix(const glm::mat4 &);

        // Sets the view matrix for the program.
        // This controls the position and orientation
        // of the camera.
        void setViewMatrix(const glm::mat4 &);
        // Sets the projection matrix for the program.
        // This specifies how to convert from 3D coordinates to
        // 2D coordinates
        void setProjectionMatrix(const glm::mat4 &);
    };
}
