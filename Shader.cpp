#include "Shader.h"
#include "TGAColor.h"
#include "model.h"
#include "our_gl.h"

Shader::Shader(Model& model, Vec3f light_dir)
: model{model}
, light_dir{std::move(light_dir)}
{
}

Vec4f Shader::vertex(int iface, int nthvert)
{
    varying_uv.set_col(nthvert, model.uv(iface, nthvert));
    varying_nrm.set_col(nthvert,
                        proj<3>((Projection * ModelView).invert_transpose() *
                                embed<4>(model.normal(iface, nthvert), 0.f)));
    Vec4f gl_Vertex =
        Projection * ModelView * embed<4>(model.vert(iface, nthvert));
    varying_tri.set_col(nthvert, gl_Vertex);
    ndc_tri.set_col(nthvert, proj<3>(gl_Vertex / gl_Vertex[3]));
    return gl_Vertex;
}

auto Shader::fragment(const Vec3f& barycentric) -> fragment_result
{
    const auto bn = (varying_nrm * barycentric).normalize();
    const auto uv = varying_uv * barycentric;

    mat<3, 3, float> A;
    A[0] = ndc_tri.col(1) - ndc_tri.col(0);
    A[1] = ndc_tri.col(2) - ndc_tri.col(0);
    A[2] = bn;

    const auto AI = A.invert();

    Vec3f i = AI * Vec3f(varying_uv[0][1] - varying_uv[0][0],
                         varying_uv[0][2] - varying_uv[0][0], 0);
    Vec3f j = AI * Vec3f(varying_uv[1][1] - varying_uv[1][0],
                         varying_uv[1][2] - varying_uv[1][0], 0);

    mat<3, 3, float> B;
    B.set_col(0, i.normalize());
    B.set_col(1, j.normalize());
    B.set_col(2, bn);

    const auto n = (B * model.normal(uv)).normalize();
    const auto diff = std::max(0.f, n * light_dir);

    return {false, model.diffuse(uv) * diff};
}
