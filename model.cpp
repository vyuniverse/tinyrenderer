#include <iostream>
#include <fstream>
#include <sstream>
#include "model.h"

Model::Model(const char *filename) : verts_(), faces_(), norms_(), uv_(), diffusemap_(), normalmap_(), specularmap_() {
    std::ifstream in;
    in.open (filename, std::ifstream::in);
    if (in.fail()) return;
    std::string line;
    while (!in.eof()) {
        std::getline(in, line);
        std::istringstream iss(line.c_str());
        char trash;
        if (!line.compare(0, 2, "v ")) {
            iss >> trash;
            Vec3f v;
            for (auto i = 0u; i<3u; ++i) iss >> v[i];
            verts_.push_back(v);
        } else if (!line.compare(0, 3, "vn ")) {
            iss >> trash >> trash;
            Vec3f n;
            for (auto i = 0u; i < 3u; ++i) iss >> n[i];
            norms_.push_back(n);
        } else if (!line.compare(0, 3, "vt ")) {
            iss >> trash >> trash;
            Vec2f uv;
            for (auto i = 0u; i < 2u; ++i) iss >> uv[i];
            uv_.push_back(uv);
        }  else if (!line.compare(0, 2, "f ")) {
            std::vector<Vec3i> f;
            Vec3i tmp;
            iss >> trash;
            while (iss >> tmp[0] >> trash >> tmp[1] >> trash >> tmp[2]) {
                for (auto i = 0u; i < 3u; ++i) tmp[i]--; // in wavefront obj all indices start at 1, not zero
                f.push_back(tmp);
            }
            faces_.push_back(f);
        }
    }
    std::cerr << "# v# " << verts_.size() << " f# "  << faces_.size() << " vt# " << uv_.size() << " vn# " << norms_.size() << std::endl;
    load_texture(filename, "_diffuse.tga", diffusemap_);
    load_texture(filename, "_nm_tangent.tga",      normalmap_);
    load_texture(filename, "_spec.tga",    specularmap_);
}

Model::~Model() {}

int Model::nverts() {
    return static_cast<int>(verts_.size());
}

int Model::nfaces() {
    return static_cast<int>(faces_.size());
}

std::vector<int> Model::face(int idx) {
    std::vector<int> face;
    for (auto i = 0u; i < faces_[static_cast<size_t>(idx)].size(); ++i) face.push_back(faces_[static_cast<size_t>(idx)][i][0]);
    return face;
}

Vec3f Model::vert(int i) {
    return verts_[static_cast<size_t>(i)];
}

Vec3f Model::vert(int iface, int nthvert) {
    return verts_[static_cast<size_t>(faces_[static_cast<size_t>(iface)][static_cast<size_t>(nthvert)][0])];
}

void Model::load_texture(std::string filename, const char *suffix, TGAImage &img) {
    std::string texfile(filename);
    size_t dot = texfile.find_last_of(".");
    if (dot!=std::string::npos) {
        texfile = texfile.substr(0,dot) + std::string(suffix);
        std::cerr << "texture file " << texfile << " loading " << (img.read_tga_file(texfile.c_str()) ? "ok" : "failed") << std::endl;
        img.flip_vertically();
    }
}

TGAColor Model::diffuse(Vec2f uvf) {
    Vec2i uv(static_cast<int>(uvf[0] * diffusemap_.get_width()), static_cast<int>(uvf[1] * diffusemap_.get_height()));
    return diffusemap_.get(uv[0], uv[1]);
}

Vec3f Model::normal(Vec2f uvf) {
    Vec2i uv(static_cast<int>(uvf[0] * normalmap_.get_width()), static_cast<int>(uvf[1] * normalmap_.get_height()));
    TGAColor c = normalmap_.get(uv[0], uv[1]);
    Vec3f res;
    for (auto i = 0u; i < 3u; ++i)
        res[2u - i] = static_cast<float>(c[static_cast<int>(i)]) / 255.f * 2.f - 1.f;
    return res;
}

Vec2f Model::uv(int iface, int nthvert) {
    return uv_[static_cast<size_t>(faces_[static_cast<size_t>(iface)][static_cast<size_t>(nthvert)][1])];
}

float Model::specular(Vec2f uvf) {
    Vec2i uv(static_cast<int>(uvf[0] * specularmap_.get_width()), static_cast<int>(uvf[1]*specularmap_.get_height()));
    return specularmap_.get(uv[0], uv[1])[0]/1.f;
}

Vec3f Model::normal(int iface, int nthvert) {
    int idx = faces_[static_cast<size_t>(iface)][static_cast<size_t>(nthvert)][2];
    return norms_[static_cast<size_t>(idx)].normalize();
}

