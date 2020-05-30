#include <cmath>
#include <cstdlib>
#include <limits>

#include "IShader.h"
#include "TGAColor.h"
#include "our_gl.h"
#include "tgaimage.h"

Mat44f ModelView;
Mat44f Viewport;
Mat44f Projection;

void viewport(int x, int y, int w, int h)
{
    Viewport = Mat44f::identity();
    Viewport[0][3] = x + w / 2.f;
    Viewport[1][3] = y + h / 2.f;
    Viewport[2][3] = 1.f;
    Viewport[0][0] = w / 2.f;
    Viewport[1][1] = h / 2.f;
    Viewport[2][2] = 0;
}

void projection(float coeff)
{
    Projection = Mat44f::identity();
    Projection[3][2] = coeff;
}

void lookat(const Vec3f& eye, const Vec3f& center, const Vec3f& up)
{
    const Vec3f z = (eye - center).normalize();
    const Vec3f x = cross(up, z).normalize();
    const Vec3f y = cross(z, x).normalize();
    Mat44f Minv = Mat44f::identity();
    Mat44f Tr = Mat44f::identity();
    for (int i = 0; i < 3; i++)
    {
        Minv[0][i] = x[i];
        Minv[1][i] = y[i];
        Minv[2][i] = z[i];
        Tr[i][3] = -center[i];
    }
    ModelView = Minv * Tr;
}

Vec3f barycentric(const Vec2f& A, const Vec2f& B, const Vec2f& C,
                  const Vec2f& P)
{
    Vec3f s[2];
    for (int i = 2; i--;)
    {
        s[i][0] = C[i] - A[i];
        s[i][1] = B[i] - A[i];
        s[i][2] = A[i] - P[i];
    }
    const Vec3f u = cross(s[0], s[1]);
    if (std::abs(u[2]) > 1e-2) // dont forget that u[2] is integer. If it is
                               // zero then triangle ABC is degenerate
        return Vec3f(1.f - (u.x + u.y) / u.z, u.y / u.z, u.x / u.z);
    return Vec3f(-1, 1, 1); // in this case generate negative coordinates, it
                            // will be thrown away by the rasterizator
}

void triangle(const Mat43f& clipc, const IShader& shader, TGAImage& image,
              float* zbuffer)
{
    Mat34f pts =
        (Viewport * clipc)
            .transpose(); // transposed to ease access to each of the points
    Mat32f pts2;
    for (int i = 0; i < 3; i++)
        pts2[i] = proj<2>(pts[i] / pts[i][3]);

    Vec2f bboxmin(std::numeric_limits<float>::max(),
                  std::numeric_limits<float>::max());
    Vec2f bboxmax(-std::numeric_limits<float>::max(),
                  -std::numeric_limits<float>::max());
    Vec2f clamp(image.get_width() - 1, image.get_height() - 1);
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            bboxmin[j] = std::max(0.f, std::min(bboxmin[j], pts2[i][j]));
            bboxmax[j] = std::min(clamp[j], std::max(bboxmax[j], pts2[i][j]));
        }
    }
    Vec2i P;
    for (P.x = bboxmin.x; P.x <= bboxmax.x; P.x++)
    {
        for (P.y = bboxmin.y; P.y <= bboxmax.y; P.y++)
        {
            const Vec3f bc_screen = barycentric(pts2[0], pts2[1], pts2[2], P);
            Vec3f bc_clip{bc_screen.x / pts[0][3], bc_screen.y / pts[1][3],
                          bc_screen.z / pts[2][3]};
            bc_clip = bc_clip / (bc_clip.x + bc_clip.y + bc_clip.z);
            const float frag_depth = clipc[2] * bc_clip;
            if (bc_screen.x < 0 || bc_screen.y < 0 || bc_screen.z < 0 ||
                zbuffer[P.x + P.y * image.get_width()] > frag_depth)
                continue;
            if (const auto& [discard, color] = shader.fragment(bc_clip);
                !discard)
            {
                zbuffer[P.x + P.y * image.get_width()] = frag_depth;
                image.set(P.x, P.y, color);
            }
        }
    }
}
