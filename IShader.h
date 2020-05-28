#ifndef ISHADER_H
#define ISHADER_H

#include "geometry.h"

struct TGAColor;

struct IShader {
    virtual ~IShader();
    virtual Vec4f vertex(int iface, int nthvert) = 0;
    virtual bool fragment(Vec3f bar, TGAColor &color) = 0;
};

#endif

