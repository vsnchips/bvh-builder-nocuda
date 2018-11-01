
#include <iostream>
#include <stdexcept>

#include "mesh.hpp"

namespace cgra {

    void Mesh::setData(const Matrix<double> &vertices,
                       const Matrix<unsigned int> &triangles) {
        // Check to make sure that the number of columns in `vertices`
        // and `triangles` is correct
        if (vertices.numCols() != 3) {
            throw std::out_of_range("`vertices` should have 3 columns");
        }
        if (triangles.numCols() != 3) {
            throw std::out_of_range("`triangles` should have 3 columns");
        }

        // delete any GPU objects we may have
        deleteMesh();

        // Clear any existing vertex and index data we have
        m_vertices.clear();
        m_indices.clear();

        // Copy the rows of `vertices` into `m_vertices`.
        for (unsigned int r = 0; r < vertices.numRows(); r++) {
            const double *vert = vertices[r];

            // Create the vertex
            Vertex v(
                glm::vec3(vert[0], vert[1], vert[2]),
                glm::vec3(0) // Normal is zero here
            );

            // Add the vertex to m_vertices
            m_vertices.push_back(v);
        }

        // Copy the data from `triangles` into `m_indices`
        // Also calculate vertex normals
        for (unsigned int r = 0; r < triangles.numRows(); r++) {
            const unsigned int *tri = triangles[r];

            // Add each index to `m_indices`
            m_indices.push_back(tri[0]);
            m_indices.push_back(tri[1]);
            m_indices.push_back(tri[2]);

            // Get the three vertices of this triangle
            Vertex &v0 = m_vertices[tri[0]];
            Vertex &v1 = m_vertices[tri[1]];
            Vertex &v2 = m_vertices[tri[2]];

            // Calcuate two of the edges of the triangle
            glm::vec3 e1 = v2.m_position - v0.m_position;
            glm::vec3 e2 = v1.m_position - v0.m_position;

            // Use those two edges to calcuate perpendicular
            // vector to those edges. We use a counter-clockwise
            // winding order to determine which way it is facing.

            // This is not a true normal, as it does not have length
            // 1. Instead the length is proportional to the area of the
            // triangle.
            glm::vec3 normal = glm::cross(e2, e1);


            // Add the calculated normal to each vertex.
            v0.m_normal += normal;
            v1.m_normal += normal;
            v2.m_normal += normal;
        }

        // Normalize the normals for each vertex, ensuring that the length is
        // 1.
        for (Vertex &v : m_vertices) {
            v.m_normal = glm::normalize(v.m_normal);
        }
    }

    void Mesh::draw(){
      draw(GL_TRIANGLES);
    }

    void Mesh::draw(GLenum mode) {
        // Check to see if we have all the GPU objects we need to draw the
        // mesh.
        if (m_vbo == 0 || m_ibo == 0 || m_vao == 0) {
            deleteMesh(); // Make sure all the existing things are deleted

            // Create the Vertex Array Object, we do this first to make
            // sure that we don't interfere with any VAO that is already
            // bound.
            glGenVertexArrays(1, &m_vao);
            glBindVertexArray(m_vao);

            // Create the Vertex Buffer Object for the vertex data
            glGenBuffers(1, &m_vbo);
            // GL_ARRAY_BUFFER is used for vertex data
            glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

            // Calculate the size, in bytes, of the vertex list
            size_t size = sizeof(Vertex) * m_vertices.size();
            // We need to pass a `const void *` to OpenGL, so we use `reinterpret_cast`
            // to do so
            const void *vertData = reinterpret_cast<const void *>(&m_vertices[0]);
            // Copy the data from the CPU memory to the GPU
            glBufferData(GL_ARRAY_BUFFER, size, vertData, GL_STATIC_DRAW);

            // Same as above, but for the index data
            glGenBuffers(1, &m_ibo);
            // GL_ELEMENT_ARRAY_BUFFER is used for index data
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
            size = sizeof(unsigned int) * m_indices.size();
            const void *idxData = reinterpret_cast<const void *>(&m_indices[0]);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, idxData, GL_STATIC_DRAW);

            // We need to bind the VBO and IBO after binding the VAO
            glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);

            // Specify how the data is laid out.
            // Attribute 0 is the position.
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                                  reinterpret_cast<void *>(offsetof(Vertex, m_position)));
            glEnableVertexAttribArray(0);

            // Attribute 1 is the position.
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                                  reinterpret_cast<void *>(offsetof(Vertex, m_normal)));
            glEnableVertexAttribArray(1);
        }

        // Set the appropriate polygon mode for the drawing mode
        if (m_drawWireframe) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        } else {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }

        // Bind the VAO
        glBindVertexArray(m_vao);

        // Draw the mesh.
        // GL_TRIANGLES means that index buffer contains a list of triangles.
        // We're using all of the indices, starting at the beginning.
        // GL_UNSIGNED_INT tells OpenGL that the index buffer is storing unsigned
        // ints.
        glDrawElements(mode, m_indices.size(), GL_UNSIGNED_INT, 0);
    }

    void Mesh::deleteMesh() {
        // If we have a valid GPU object, delete it and
        // set it to 0. 0 is almost never a valid value for
        // and OpenGL Object handle.
        if (m_vbo != 0) {
            glDeleteBuffers(1, &m_vbo);
            m_vbo = 0;
        }
        if (m_ibo != 0) {
            glDeleteBuffers(1, &m_ibo);
            m_ibo = 0;
        }
        if (m_vao != 0) {
            glDeleteVertexArrays(1, &m_vao);
            m_vao = 0;
        }
    }
}
