#ifndef SHADER_H
#define SHADER_H

#include "IShader.h"
#include "geometry.h"

class Model;
struct TGAColor;

struct Shader : public IShader
{
public:
    Shader(Model& model, Vec3f light_dir);

    const Model& model;
    const Vec3f light_dir;

    Mat23f varying_uv;  // triangle uv coordinates, written by the
                        // vertex shader, read by the fragment shader
    Mat43f varying_tri; // triangle coordinates (clip coordinates),
                        // written by VS, read by fragment shader
    Mat33f
        varying_nrm; // normal per vertex to be interpolated by fragment shader
    Mat33f ndc_tri;  // triangle in normalized device coordinates

    virtual Vec4f vertex(int iface, int nthvert) override;

    virtual fragment_result fragment(const Vec3f& barycentric) const override;
};

#endif
