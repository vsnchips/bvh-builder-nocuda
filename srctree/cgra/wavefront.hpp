#pragma once

#include <vector>

#include <glm/glm.hpp>

namespace cgra {

    class Wavefront {
    public:

        // Enum that represents the type of vertex
        // Wavefront vertices must have a position,
        // but normals and texture coordinates are
        // optional
        enum VertexType {
            VT_P,   // Only position
            VT_PT,  // Position + Texture Coordinate
            VT_PN,  // Position + Normal
            VT_PTN  // Position + Texture Coordinate + Normal
        };
        // Struct representing a vertex.
        // Stores indices into the `m_positions`, `m_normals`
        // and `m_texCoords` lists.
        // Wavefront indices start at 1, instead of 0.
        struct Vertex {
            // Type of vertex
            VertexType m_type;

            // Index of position value
            unsigned int m_p;
            // Index of texture coordinate value, if it has one.
            unsigned int m_t;
            // Index of normal value, if it has one.
            unsigned int m_n;

            Vertex(VertexType type,
                   unsigned int p,
                   unsigned int t,
                   unsigned int n)
                : m_type(type), m_p(p), m_t(t), m_n(n) { }

            // Returns true if this vertex has a texture coordinate
            bool hasTexCoord() const { return m_type == VT_PT || m_type == VT_PTN; }
            // Returns true if this vertex has a normal
            bool hasNormal() const { return m_type == VT_PN || m_type == VT_PTN; }

        };

        // Struct representing a single face.
        // Wavefront faces must have atleast 3 vertices, but can have more.
        struct Face {
            std::vector<Vertex> m_vertices;
        };

        // A list of vertex positions
        std::vector<glm::vec4> m_positions;
        // A list of vertex normals
        std::vector<glm::vec3> m_normals;
        // A list of vertex texture coordinates
        std::vector<glm::vec3> m_texCoords;

        // A list of faces.
        std::vector<Face> m_faces;

        float range;

        // Load a wavefront file.
        static Wavefront load(const char *filename, bool expectTriangles = true);

        // Helper functions for querying vertex information

        glm::vec4 position(unsigned int i) const {
            return m_positions[size_t(i) - 1];
        }
        glm::vec3 normal(unsigned int i) const {
            return m_normals[size_t(i) - 1];
        }
        glm::vec3 texCoord(unsigned int i) const {
            return m_texCoords[size_t(i) - 1];
        }
    };

}
