#ifndef TGA_HEADER_H
#define TGA_HEADER_H

#pragma pack(push,1)
struct TGA_Header {
    char idlength;
    char colormaptype;
    char datatypecode;
    short colormaporigin;
    short colormaplength;
    char colormapdepth;
    short x_origin;
    short y_origin;
    short width;
    short height;
    char  bitsperpixel;
    char  imagedescriptor;
};
#pragma pack(pop)

#endif

