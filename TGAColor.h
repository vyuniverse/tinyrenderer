#ifndef TGA_COLOR_H
#define TGA_COLOR_H

struct TGAColor
{
    unsigned char bgra[4];
    unsigned char bytespp;

    TGAColor()
    : bgra()
    , bytespp(1)
    {
        for (int i = 0; i < 4; i++)
            bgra[i] = 0;
    }

    TGAColor(unsigned char R, unsigned char G, unsigned char B,
             unsigned char A = 255)
    : bgra()
    , bytespp(4)
    {
        bgra[0] = B;
        bgra[1] = G;
        bgra[2] = R;
        bgra[3] = A;
    }

    TGAColor(unsigned char v)
    : bgra()
    , bytespp(1)
    {
        for (int i = 0; i < 4; i++)
            bgra[i] = 0;
        bgra[0] = v;
    }

    TGAColor(const unsigned char* p, unsigned char bpp)
    : bgra()
    , bytespp(bpp)
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

    unsigned char& operator[](const int i) { return bgra[i]; }

    TGAColor operator*(float intensity) const
    {
        TGAColor res = *this;
        intensity =
            (intensity > 1.f ? 1.f : (intensity < 0.f ? 0.f : intensity));
        for (int i = 0; i < 4; i++)
            res.bgra[i] = bgra[i] * intensity;
        return res;
    }
};

#endif
