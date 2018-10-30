#pragma once

#include <vector>

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "matrix.hpp"
#include "glm/glm.hpp"

namespace cgra {

    class Mesh {
        // Represents a single vertex in the mesh

    public:
        struct Vertex {
            // The position of the vertex
            glm::vec3 m_position;
            // The normal at the vertex
            glm::vec3 m_normal;

            glm::vec2 m_uv;

            Vertex(glm::vec3 pos, glm::vec3 norm) :
                m_position(pos), m_normal(norm) { }
        };

        // A list of all the vertices in the mesh
        std::vector<Vertex> m_vertices;
        // A list of indices into m_vertices.
        // Every three indices represents a triangle,
        // For example, the list:
        //    [0, 1, 2, 0, 2, 3]
        // represents two triangles. The first triangle
        // uses the first, second and third vertices;
        // the second triangle uses the first, third and
        // fourth triangles.
        std::vector<unsigned int> m_indices;

       float maxdist;

        // Whether or not to draw this mesh as a wireframe
        bool m_drawWireframe;

        /********************************************************
         * GPU Objects                                          *
         *                                                      *
         * These represent data that is stored on the GPU       *
         ********************************************************/

        // The Vertex Array Object. It describes how the data is
        // represented in the buffers.
        GLuint m_vao;
        // The Vertex Buffer Object, this stores the actual vertex
        // data.
        GLuint m_vbo;
        // The Index Buffer Object, this stores a list of indices
        // that represent triangles
        GLuint m_ibo;


        // Set the mesh data using two Matrices.
        //
        // `vertices` is an n x 3 matrix of vertex positions
        // `triangles` is an n x 3 matrix of triangles using
        //    indices into the rows of `vertices`
        void setData(const Matrix<double> &vertices,
                     const Matrix<unsigned int> &triangles);

        // Set whether or not to draw this mesh as a wireframe.
        // true means that the mesh will be drawn as a wireframe.
        void setDrawWireframe(bool wireframe) {
            m_drawWireframe = wireframe;
        }

        // Get whether this mesh will be drawn as a wireframe or not
        bool getDrawWireframe() const {
            return m_drawWireframe;
        }

        // Draw the mesh
        void draw();
        void draw(GLenum mode);

        // Default constructor
        Mesh()
            : m_drawWireframe(false),
              m_vao(0), m_vbo(0), m_ibo(0) { }

        // Copy constructor.
        // This copies the vertex and index data, but
        // doesn't copy the GPU objects.
        Mesh(const Mesh &m) :
            m_vertices(m.m_vertices),
            m_indices(m.m_indices),
            m_drawWireframe(m.m_drawWireframe),
            m_vao(0), m_vbo(0), m_ibo(0) { }

        // Copy assignment, same as the copy constructor
        // except the GPU objects are released first
        Mesh & operator=(const Mesh &m) {
            m_vertices = m.m_vertices;
            m_indices = m.m_indices;
            m_drawWireframe = m.m_drawWireframe;

            deleteMesh();

            m_vao = 0;
            m_vbo = 0;
            m_ibo = 0;
            return *this;
        }

        // Move constructor
        // Moves the vertex and index data from the source
        // as well as moving the GPU objects.
        Mesh(Mesh &&m)
            : m_vertices(std::move(m.m_vertices)),
              m_indices(std::move(m.m_indices)),
              m_drawWireframe(m.m_drawWireframe),
              m_vao(m.m_vao), m_vbo(m.m_vbo), m_ibo(m.m_ibo) {
            m.m_vao = 0;
            m.m_vbo = 0;
            m.m_ibo = 0;
        }

        // Move assignment, same as the move constructor
        // except the GPU objects are released first.
        Mesh & operator = (Mesh &&m) {
            m_vertices = std::move(m.m_vertices);
            m_indices = std::move(m.m_indices);
            m_drawWireframe = m.m_drawWireframe;

            deleteMesh();

            m_vbo = m.m_vbo;
            m_vao = m.m_vao;
            m_ibo = m.m_ibo;

            m.m_vbo = 0;
            m.m_vao = 0;
            m.m_ibo = 0;

            return *this;
        }

        // Deletes the GPU objects if necessary
        void deleteMesh();

        // Destructor. Called when the object goes out of
        // scope. It frees up the GPU objects.
        ~Mesh() {
            deleteMesh();
        }
    };

}
