#ifndef ISHADER_H
#define ISHADER_H

#include "TGAColor.h"
#include "geometry.h"

struct IShader
{
    virtual ~IShader();
    virtual Vec4f vertex(int iface, int nthvert) = 0;

    struct fragment_result
    {
        bool discard;
        TGAColor colour;
    };

    virtual fragment_result fragment(const Vec3f& barycentric) const = 0;
};

#endif
