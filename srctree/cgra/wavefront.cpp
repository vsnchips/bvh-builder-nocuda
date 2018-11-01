#include <iostream>
#include <fstream>
#include <sstream>

#include <stdexcept>

#include "wavefront.hpp"

namespace cgra {
    // Helper function for checking the next character in
    // a stream and advancing if it matches the given character
    static bool eat(std::istream &stream, char c) {
        if (!stream.good()) return false;
        if (stream.peek() == c) {
            stream.ignore(1);
            return true;
        } else {
            return false;
        }
    }

    Wavefront Wavefront::load(const char *filename, bool expectTriangles) {
        Wavefront wavefront;

        // Open the file
        std::ifstream file(filename, std::ios::in);
        if (!file.is_open()) {
            throw std::runtime_error("Error: could not open file for reading");
        } else std::cout << "\nfile open";

        std::string line;
        std::string mode;
        size_t lineNumber = 0;

        if (file.good()) std::cout << "\n good file!\n";

        wavefront.range=0.;
        while (file.good()) {
            // Get the next line in the file
            std::getline(file, line);
            lineNumber++;

            // Skip comments
            if (line[0] == '#') continue;

            // Create a stream for the line
            std::istringstream lineStream(line);

            // Clear the previous mode
            mode.clear();
            lineStream >> mode;

            // Empty line or comment
            if (mode.size() == 0 || mode[0] == '#') continue;

            if (mode == "v") {
                // Vertex Position
                // Wavefront positions can have three or four components
                float x, y, z, w;
                w = 1.0f;
                lineStream >> x >> y >> z >> w;
                wavefront.m_positions.push_back(glm::vec4(x, y, z, w));
                float len = glm::length(glm::vec3(x,y,z));
                if (len > wavefront.range) wavefront.range=len;
               // std::cout << "\nPushing vertex number " << wavefront.m_positions.size();
            } else if (mode == "vt") {
                // Vertex Texture Coordinate
                // Wavefront texture coordinates can have two or three components
                float u, v, w;
                w = 0.0f;

                lineStream >> u >> v >> w;
                wavefront.m_texCoords.push_back(glm::vec3(u, v, w));
            } else if (mode == "vn") {
                // Vertex Normal
                // Wavefront normals always have three components
                float x, y, z;
                lineStream >> x >> y >> z;
                wavefront.m_normals.push_back(glm::vec3(x, y, z));
            } else if (mode == "f") {
                // Face specification
                // Wavefront faces consist of 3 or more vertices.
                // Each vertex must have a position, but can also have an
                // optional normal and/or texture coordinate. The indices are
                // separated by '/'. There are 4 cases:
                //
                //    pos
                //    pos/texCoord
                //    pos//normal
                //    pos/texCoord/normal

                Face f;
                // Reserve space for at least 3 vertices
                f.m_vertices.reserve(3);
                while (lineStream.good()) {
                    VertexType type = VT_P;
                    int p, t = 0, n = 0;

                    // Read the position index
                    lineStream >> p;
                    if (lineStream.fail()) break;

                    // Use the `eat` helper function to check for the
                    // presence of '/' characters that indicate what kind
                    // index we have
                    if (eat(lineStream, '/')) {
                        if (eat(lineStream, '/')) {
                            type = VT_PN;
                            lineStream >> n;
                        } else {
                            type = VT_PT;
                            lineStream >> t;
                            if (eat(lineStream, '/')) {
                                type = VT_PTN;
                                lineStream >> n;
                            }
                        }
                    }

                    // Convert from relative to absolute indices
                    // Wavefront allows negative indices, which are intepreted to be
                    // relative to the current list of values.
                    // -1 is the most recent value, -2 is the one before that, etc.
                    if (p < 0) p = wavefront.m_positions.size() - (p + 1);
                    if (t < 0) t = wavefront.m_texCoords.size() - (t + 1);
                    if (n < 0) n = wavefront.m_normals.size() - (n + 1);

                    f.m_vertices.emplace_back(type, p, t, n);
                }

                if (f.m_vertices.size() < 3) {
                    // Faces shouldn't have less than three vertices, ever.
                    std::cerr << '[' << lineNumber << "] Warning: face with less than 3 vertices" << std::endl;
                } else {
                    // Faces can have more than three vertices, but that often isn't expected, so warn about
                    // it if we expect a triangluated mesh.
                    if (expectTriangles && f.m_vertices.size() > 3) {
                        std::cerr << '[' << lineNumber << "] Warning: face with more than 3 vertices" << std::endl;
                    }
                    // Add the face to the list of faces
                    wavefront.m_faces.push_back(f);
                }
            } else {
                // Skip unrecognized things in the file
                continue;
            }
        }

        return wavefront;
    }
}
