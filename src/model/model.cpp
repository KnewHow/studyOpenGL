// #include "model.hpp"

// #include<iostream>
// #include<fstream>
// #include<sstream>

// Model::Model(const std::string& path) 
//     :_verteces(), _uvs(), _normals(), _faces_vert(), _faces_tex(), _faces_norm()
// {
//      std::ifstream in;
//     in.open(path, std::ifstream::in);
//     if(in.fail()) return;
//     std::string line;
//     while(!in.eof()) {
//         std::getline(in, line);
//         std::istringstream iss(line.c_str());
//         char trash;
//         if(!line.compare(0, 2, "v ")) {
//             iss >> trash;
//             glm::vec3 v;
//             for(int i = 0; i < 3; i++) iss >> v[i];
//             _verteces.push_back(v);
//         } else if(!line.compare(0, 3, "vn ")) {
//             iss >> trash >> trash;
//             glm::vec3 n;
//             for(int i = 0; i < 3; i++) iss >> n[i];
//             _normals.push_back();
//         } else if(!line.compare(0, 3, "vt ")) {
//             iss >> trash >> trash;
//             glm::vec2 uv;
//             for(int i = 0; i < 2; i++) iss >> uv[i];
//             _uvs.push_back(uv);
//         } else if(!line.compare(0, 2, "f ")) {
//             iss >> trash;
//             int v, t, n;
//             int cnt = 0;
//             while(iss >> v >> trash >> t >> trash >> n) {
//                 _faces_vert.push_back(--v);
//                 _faces_tex.push_back(--t);
//                 _faces_norm.push_back(--n);
//                 cnt++;
//             }
//             if(cnt != 3) {
//                 std::cerr << "Error: the obj file is supposed be triangled" << std::endl;
//                 in.close();
//                 return;
//             }

//         }
//     }

//     in.close();
//     // std::cerr << "# v# " << nverts() << " f# "  << nfaces() << " vt# " << uv_.size() << " vn# " << norms_.size() << std::endl;
//     // load_texture(filename, "_diffuse.tga", diffusemap_);
//     // load_texture(filename, "_nm_tangent.tga", normalmap_);
//     // load_texture(filename, "_spec.tga", specularmap_);
// }