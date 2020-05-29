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

    mat<2, 3, float> varying_uv;  // triangle uv coordinates, written by the
                                  // vertex shader, read by the fragment shader
    mat<4, 3, float> varying_tri; // triangle coordinates (clip coordinates),
                                  // written by VS, read by fragment shader
    mat<3, 3, float>
        varying_nrm; // normal per vertex to be interpolated by fragment shader
    mat<3, 3, float> ndc_tri; // triangle in normalized device coordinates

    virtual Vec4f vertex(int iface, int nthvert) override;

    virtual fragment_result fragment(const Vec3f& barycentric) const override;
};

#endif
