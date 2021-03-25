#pragma once

#include <string>
#include <vector>
#include "vector.hpp"

class Model {
private:
    std::vector<math::vec3f> _verteces;
    std::vector<math::vec2f> _uvs;
    std::vector<math::vec3f> _normals;

    std::vector<int> _faces_vert;
    std::vector<int> _faces_tex;
    std::vector<int> _faces_norm;

public:
    Model(const std::string& path);
    ~Model();
    inline int nvertes() { return _verteces.size(); }
    inline int nfaces() { return _faces_vert.size() / 3; }
    inline math::vec4f vert(int iface, int nthvert) { return math::embed<float, 4>(_verteces[iface * 3 + nthvert]); }
    std::vector<math::vec4f> getTriangles(); // get all triangles
    std::vector<math::vec4f> getVertes4f();
};