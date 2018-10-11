#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include <stdexcept>

#include "shader.hpp"

namespace cgra {

    // Helper function for compiling a single shader stage
    static GLuint compileShader(const char *src, GLenum type);

    // Helper function for reading a file into a string
    static std::string read_all(std::ifstream &in) {
        std::stringstream sstr;
        sstr << in.rdbuf();
        return sstr.str();
    }

    // Helper function for reading a file into a string
    static std::string read_all(const char *filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::ostringstream msgStream;
            msgStream << "Cannot open file '" << filename << "'";
            printf("%s",msgStream.str());
           // throw std::runtime_error(msgStream.str());
           return "";
        }
        return read_all(file);
    }

    Program Program::load_program(const char *vsFile, const char *fsFile) {
        // Read the contents of the vertex and fragment shader files into memory
        std::string vsSrc = read_all(vsFile);
        std::string fsSrc = read_all(fsFile);

        // Compile the shader stages.

        GLuint vs = compileShader(vsSrc.c_str(), GL_VERTEX_SHADER);
        GLuint fs = compileShader(fsSrc.c_str(), GL_FRAGMENT_SHADER);

        // Create a new program object
            glDeleteProgram(m_glprogram);
        m_glprogram = glCreateProgram();

        // Attach the shader stages to the program
        glAttachShader(m_glprogram, vs);
        glAttachShader(m_glprogram, fs);

        // Set the attribute locations
        // 0 is the position.
        // 1 is the normal.
        glBindAttribLocation(m_glprogram, 0, "vertPosition");
        glBindAttribLocation(m_glprogram, 1, "vertNormal");

        // Link the stages together. This must be done
        // after setting the program
        glLinkProgram(m_glprogram);

        // Check to see that the linking was successful
        GLint isLinked = 0;
        m_islinked = false;
        glGetProgramiv(m_glprogram, GL_LINK_STATUS, &isLinked);
        if (isLinked == GL_FALSE) {
            // Get the length of the error message
            GLint maxLength = 0;
            glGetProgramiv(m_glprogram, GL_INFO_LOG_LENGTH, &maxLength);

            //The maxLength includes the NULL character
            std::string infoLog(maxLength, 0);
            glGetProgramInfoLog(m_glprogram, maxLength, &maxLength, &infoLog[0]);

            // Print an error message
            std::cerr << "Failed to link program:\n" << infoLog << std::endl;

            //We don't need the shader anymore.
            glDeleteProgram(m_glprogram);
            glDeleteShader(vs);
            glDeleteShader(fs);

        } 
        
        m_islinked = (isLinked == GL_TRUE);
        std::cout << " Link was successful, we can detach the shader stages from the program now\n" ; 
        glDetachShader(m_glprogram, vs);
        glDetachShader(m_glprogram, fs);

        // We can also delete the shader objects, since we won't be using
        // them again
        glDeleteShader(vs);
        glDeleteShader(fs);

        auto prog = Program(m_glprogram);

        // Set default values for the matrices
        prog.setModelMatrix(glm::mat4(1));
        prog.setViewMatrix(glm::mat4(1));
        prog.setProjectionMatrix(glm::mat4(1));

        return prog;
    }

    void Program::use() {
        if (m_glprogram != 0) {
            glUseProgram(m_glprogram);
        }
    }

    void Program::setModelMatrix(const glm::mat4 &mat) {
        if (m_glprogram == 0) return;
        use();

        // Get the location of the "modelMat" uniform variable
        GLint modelLoc = glGetUniformLocation(m_glprogram, "modelMat");
        // Set the value of "modelMat"
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mat[0][0]);
    }

    void Program::setViewMatrix(const glm::mat4 &mat) {
        if (m_glprogram == 0) return;
        use();

        GLint viewLoc = glGetUniformLocation(m_glprogram, "viewMat");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &mat[0][0]);
    }

    void Program::setProjectionMatrix(const glm::mat4 &mat) {
        if (m_glprogram == 0) return;
        use();

        GLint projectionLoc = glGetUniformLocation(m_glprogram, "projectionMat");
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, &mat[0][0]);
    }

    static GLuint compileShader(const char *src, GLenum type) {
        // Create the shader object
        GLuint shader = glCreateShader(type);
        // Load the shader source into the object
        glShaderSource(shader, 1, &src, nullptr);
        // Compile the shader
        glCompileShader(shader);

        // Check to see if the compilation was successful
        GLint isCompiled = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);

        if (isCompiled == GL_FALSE) {
            // Get the length of the error message
            GLint maxLength = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

            //The maxLength includes the NULL character
            std::string infoLog(maxLength, 0);
            glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

            // Print the error mesage
            std::cerr << "Failed to compile shader:\n" << infoLog << std::endl;

            //We don't need the shader anymore.
            glDeleteShader(shader);

            // Abort the process
            //abort();
        }

        return shader;
    }
}
