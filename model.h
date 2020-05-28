#ifndef __MODEL_H__
#define __MODEL_H__
#include <vector>
#include <string>
#include "geometry.h"
#include "tgaimage.h"

class Model {
private:
    std::vector<Vec3f> verts_;
    std::vector<std::vector<Vec3i> > faces_; // attention, this Vec3i means vertex/uv/normal
    std::vector<Vec3f> norms_;
    std::vector<Vec2f> uv_;
    TGAImage diffusemap_;
    TGAImage normalmap_;
    TGAImage specularmap_;
public:
    Model(const std::string& object_filename,
        const std::string& diffuse_map_filename,
        const std::string& normal_map_filename,
        const std::string& specular_map_filename);
    ~Model();
    int nverts() const;
    int nfaces() const;
    Vec3f normal(int iface, int nthvert) const;
    Vec3f normal(Vec2f uv) const;
    Vec3f vert(int i) const;
    Vec3f vert(int iface, int nthvert) const;
    Vec2f uv(int iface, int nthvert) const;
    TGAColor diffuse(Vec2f uv) const;
    float specular(Vec2f uv) const;
    std::vector<int> face(int idx) const;
};
#endif //__MODEL_H__

