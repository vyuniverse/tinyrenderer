#ifndef TGA_COLOR_H
#define TGA_COLOR_H

struct TGAColor
{
    unsigned char bgra[4];
    unsigned char bytespp;

    TGAColor();

    TGAColor(unsigned char R, unsigned char G, unsigned char B,
             unsigned char A = 255);
    TGAColor(unsigned char v);
    TGAColor(const unsigned char* p, unsigned char bpp);

    unsigned char& operator[](const int i);
    TGAColor operator*(float intensity) const;
};

#endif
