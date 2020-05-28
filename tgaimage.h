#ifndef __IMAGE_H__
#define __IMAGE_H__

#include <iosfwd>
#include <vector>

struct TGAColor;

class TGAImage {
protected:
    std::vector<unsigned char> data;
    int width;
    int height;
    int bytespp;

    bool   load_rle_data(std::ifstream &in);
    bool unload_rle_data(std::ofstream &out) const;
public:
    enum Format {
        GRAYSCALE=1, RGB=3, RGBA=4
    };

    TGAImage();
    TGAImage(int w, int h, int bpp);
    TGAImage(const TGAImage &img);
    bool read_tga_file(const char *filename);
    bool write_tga_file(const char *filename, bool rle=true) const;
    bool flip_horizontally();
    bool flip_vertically();
    bool scale(int w, int h);
    TGAColor get(int x, int y) const;
    bool set(int x, int y, TGAColor &c);
    bool set(int x, int y, const TGAColor &c);
    TGAImage & operator =(const TGAImage &img);
    int get_width() const;
    int get_height() const;
    int get_bytespp() const;
    const unsigned char *buffer() const;
    void clear();
};

#endif //__IMAGE_H__

