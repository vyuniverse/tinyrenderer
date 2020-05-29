#include "model.h"
#include "TGAColor.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>

namespace
{
void load_texture(const std::string& filename, TGAImage& image)
{
    std::cerr << "texture file " << filename << " loading "
              << (image.read_tga_file(filename.c_str()) ? "ok" : "failed")
              << '\n';
    image.flip_vertically();
}
} // namespace

Model::Model(const std::string& object_filename,
             const std::string& diffuse_map_filename,
             const std::string& normal_map_filename,
             const std::string& specular_map_filename)
: verts_{}
, faces_{}
, norms_{}
, uv_{}
, diffusemap_{}
, normalmap_{}
, specularmap_{}
{
    std::ifstream in;
    in.open(object_filename, std::ifstream::in);
    if (in.fail())
        return;
    std::string line;
    while (!in.eof())
    {
        std::getline(in, line);
        std::istringstream iss(line.c_str());
        char trash;
        if (!line.compare(0, 2, "v "))
        {
            iss >> trash;
            Vec3f v;
            for (int i = 0; i < 3; i++)
                iss >> v[i];
            verts_.push_back(v);
        }
        else if (!line.compare(0, 3, "vn "))
        {
            iss >> trash >> trash;
            Vec3f n;
            for (int i = 0; i < 3; i++)
                iss >> n[i];
            norms_.push_back(n.normalize());
        }
        else if (!line.compare(0, 3, "vt "))
        {
            iss >> trash >> trash;
            Vec2f uv;
            for (int i = 0; i < 2; i++)
                iss >> uv[i];
            uv_.push_back(uv);
        }
        else if (!line.compare(0, 2, "f "))
        {
            std::vector<Vec3i> f;
            Vec3i tmp;
            iss >> trash;
            while (iss >> tmp[0] >> trash >> tmp[1] >> trash >> tmp[2])
            {
                for (int i = 0; i < 3; i++)
                    tmp[i]--; // in wavefront obj all indices start at 1, not
                              // zero
                f.push_back(tmp);
            }
            faces_.push_back(f);
        }
    }
    std::cerr << "# v# " << verts_.size() << " f# " << faces_.size() << " vt# "
              << uv_.size() << " vn# " << norms_.size() << std::endl;

    load_texture(diffuse_map_filename, diffusemap_);
    load_texture(normal_map_filename, normalmap_);
    load_texture(specular_map_filename, specularmap_);
}

Model::~Model() = default;

int Model::nverts() const
{
    return (int)verts_.size();
}

int Model::nfaces() const
{
    return (int)faces_.size();
}

std::vector<int> Model::face(int idx) const
{
    std::vector<int> face;
    for (int i = 0; i < (int)faces_[idx].size(); i++)
        face.push_back(faces_[idx][i][0]);
    return face;
}

Vec3f Model::vert(int i) const
{
    return verts_[i];
}

Vec3f Model::vert(int iface, int nthvert) const
{
    return verts_[faces_[iface][nthvert][0]];
}

TGAColor Model::diffuse(Vec2f uvf) const
{
    if (!diffusemap_.buffer())
    {
        return {255, 255, 255};
    }
    Vec2i uv(uvf[0] * diffusemap_.get_width(),
             uvf[1] * diffusemap_.get_height());
    return diffusemap_.get(uv[0], uv[1]);
}

Vec3f Model::normal(Vec2f uvf) const
{
    Vec2i uv(uvf[0] * normalmap_.get_width(), uvf[1] * normalmap_.get_height());
    TGAColor c = normalmap_.get(uv[0], uv[1]);
    Vec3f res;
    for (int i = 0; i < 3; i++)
        res[2 - i] = (float)c[i] / 255.f * 2.f - 1.f;
    return res;
}

Vec2f Model::uv(int iface, int nthvert) const
{
    return uv_[faces_[iface][nthvert][1]];
}

float Model::specular(Vec2f uvf) const
{
    Vec2i uv(uvf[0] * specularmap_.get_width(),
             uvf[1] * specularmap_.get_height());
    return specularmap_.get(uv[0], uv[1])[0] / 1.f;
}

Vec3f Model::normal(int iface, int nthvert) const
{
    int idx = faces_[iface][nthvert][2];
    return norms_[idx];
}
