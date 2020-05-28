#ifndef __OUR_GL_H__
#define __OUR_GL_H__
#include "IShader.h"
#include "tgaimage.h"
#include "geometry.h"

extern Matrix ModelView;
extern Matrix Projection;

void viewport(int x, int y, int w, int h);
void projection(float coeff=0.f); // coeff = -1/c
void lookat(Vec3f eye, Vec3f center, Vec3f up);

//void triangle(Vec4f *pts, IShader &shader, TGAImage &image, float *zbuffer);
struct IShader;
void triangle(mat<4,3,float> &pts, IShader &shader, TGAImage &image, float *zbuffer);
#endif //__OUR_GL_H__

