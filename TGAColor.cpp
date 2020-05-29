#include "TGAColor.h"

TGAColor::TGAColor()
: bgra{}
, bytespp{1}
{
}

TGAColor::TGAColor(unsigned char R, unsigned char G, unsigned char B,
                   unsigned char A)
: bgra{B, G, R, A}
, bytespp{4}
{
}

TGAColor::TGAColor(unsigned char v)
: bgra{v, 0, 0, 0}
, bytespp(1)
{
}

TGAColor::TGAColor(const unsigned char* p, unsigned char bpp)
: bgra{}
, bytespp{bpp}
{
    for (int i = 0; i < (int)bpp; i++)
    {
        bgra[i] = p[i];
    }
    for (int i = bpp; i < 4; i++)
    {
        bgra[i] = 0;
    }
}

unsigned char& TGAColor::operator[](const int i)
{
    return bgra[i];
}

unsigned char TGAColor::operator[](int i) const
{
    return bgra[i];
}

TGAColor TGAColor::operator*(float intensity) const
{
    TGAColor res = *this;
    intensity = (intensity > 1.f ? 1.f : (intensity < 0.f ? 0.f : intensity));
    for (int i = 0; i < 4; i++)
        res.bgra[i] = bgra[i] * intensity;
    return res;
}
