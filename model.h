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
    void load_texture(std::string filename, const char *suffix, TGAImage &img);
public:
    Model(const char *filename);
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

