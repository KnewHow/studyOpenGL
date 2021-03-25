#include "model.hpp"

#include<iostream>
#include<fstream>
#include<sstream>

Model::Model(const std::string& path) 
    :_verteces(), _uvs(), _normals(), _faces_vert(), _faces_tex(), _faces_norm()
{
     std::ifstream in;
    in.open(path, std::ifstream::in);
    if(in.fail()) return;
    std::string line;
    while(!in.eof()) {
        std::getline(in, line);
        std::istringstream iss(line.c_str());
        char trash;
        if(!line.compare(0, 2, "v ")) {
            iss >> trash;
            math::vec3f v;
            for(int i = 0; i < 3; i++) iss >> v[i];
            _verteces.push_back(v);
        } else if(!line.compare(0, 3, "vn ")) {
            iss >> trash >> trash;
            math::vec3f n;
            for(int i = 0; i < 3; i++) iss >> n[i];
            _normals.push_back(n.normalize());
        } else if(!line.compare(0, 3, "vt ")) {
            iss >> trash >> trash;
            math::vec2f uv;
            for(int i = 0; i < 2; i++) iss >> uv[i];
            _uvs.push_back(uv);
        } else if(!line.compare(0, 2, "f ")) {
            iss >> trash;
            int v, t, n;
            int cnt = 0;
            while(iss >> v >> trash >> t >> trash >> n) {
                _faces_vert.push_back(--v);
                _faces_tex.push_back(--t);
                _faces_norm.push_back(--n);
                cnt++;
            }
            if(cnt != 3) {
                std::cerr << "Error: the obj file is supposed be triangled" << std::endl;
                in.close();
                return;
            }

        }
    }

    in.close();
    std::cerr << "# v# " << nvertes() << " f# "  << nfaces() << " vt# " << _uvs.size() << " vn# " << _normals.size() << std::endl;
    // load_texture(filename, "_diffuse.tga", diffusemap_);
    // load_texture(filename, "_nm_tangent.tga", normalmap_);
    // load_texture(filename, "_spec.tga", specularmap_);
}

Model::~Model() {}

std::vector<math::vec4f> Model::getVertes4f() {
    std::vector<math::vec4f> res;
    res.reserve(_verteces.size());
    for(const auto& v: _verteces) {
        res.push_back(math::embed<float, 4>(v, 1.0f));
    }
    return res;
}

std::vector<math::vec4f> Model::getTriangles() {
    std::vector<math::vec4f> res;
    for(int i = 0;  i < nfaces(); i++) {
        for(int j = 0; j < 3; j++) {
            auto t = vert(i, j) * 0.25f;
            math::vec4f v = math::vec4f(t.x, t.y, t.z, 1.0f);
            res.push_back(v);
        }
    }
    return res;
}