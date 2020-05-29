#ifndef __OUR_GL_H__
#define __OUR_GL_H__
#include "geometry.h"

extern Matrix ModelView;
extern Matrix Projection;

void viewport(int x, int y, int w, int h);
void projection(float coeff = 0.f); // coeff = -1/c
void lookat(const Vec3f& eye, const Vec3f& center, const Vec3f& up);

struct IShader;
class TGAImage;
void triangle(const mat<4, 3, float>& pts, const IShader& shader,
              TGAImage& image, float* zbuffer);
#endif //__OUR_GL_H__
