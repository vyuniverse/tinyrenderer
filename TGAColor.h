#ifndef TGA_COLOR_H
#define TGA_COLOR_H

#include <array>

struct TGAColor
{
    std::array<unsigned char, 4> bgra;
    unsigned char bytespp;

    TGAColor();

    TGAColor(unsigned char R,
             unsigned char G,
             unsigned char B,
             unsigned char A = 255);
    TGAColor(unsigned char v);
    TGAColor(const unsigned char* p, unsigned char bpp);

    unsigned char& operator[](const int i);
    unsigned char operator[](int i) const;
    TGAColor operator*(float intensity) const;
};

#endif
