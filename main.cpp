#include "IShader.h"
#include "Shader.h"
#include "TGAColor.h"
#include "geometry.h"
#include "model.h"
#include "our_gl.h"
#include "tgaimage.h"
#include <exception>
#include <iostream>
#include <limits>
#include <vector>

constexpr int width = 800;
constexpr int height = 800;

Vec3f light_dir(1, 1, 1);
Vec3f eye(1, 1, 3);
Vec3f center(0, 0, 0);
Vec3f up(0, 1, 0);

std::string add_suffix_to_filename(std::string filename, const char* suffix)
{
    if (const auto dot = filename.find_last_of("."); dot != std::string::npos)
    {
        return filename.substr(0, dot) + suffix;
    }
    else
    {
        throw std::runtime_error{"add_suffix: no extension found"};
    }
}

auto make_diffuse_map_filename(std::string base_filename)
{
    const char* diffuse_map_suffix = "_diffuse.tga";
    return add_suffix_to_filename(std::move(base_filename), diffuse_map_suffix);
}

auto make_normal_map_filename(std::string base_filename)
{
    const char* normal_map_suffix = "_nm_tangent.tga";
    return add_suffix_to_filename(std::move(base_filename), normal_map_suffix);
}

auto make_specular_map_filename(std::string base_filename)
{
    const char* specular_map_suffix = "_spec.tga";
    return add_suffix_to_filename(std::move(base_filename),
                                  specular_map_suffix);
}

int main(int argc, char** argv)
{
    try
    {
        if (2 > argc)
        {
            std::cerr << "Usage: " << argv[0] << " obj/model.obj" << std::endl;
            return 1;
        }

        std::vector<float> zbuffer(width * height,
                                   -std::numeric_limits<float>::max());

        TGAImage frame(width, height, TGAImage::RGB);
        lookat(eye, center, up);
        viewport(width / 8, height / 8, width * 3 / 4, height * 3 / 4);
        projection(-1.f / (eye - center).norm());
        light_dir = proj<3>((Projection * ModelView * embed<4>(light_dir, 0.f)))
                        .normalize();

        for (int m = 1; m < argc; m++)
        {
            std::string model_filename{argv[m]};
            Model model{model_filename,
                        make_diffuse_map_filename(model_filename),
                        make_normal_map_filename(model_filename),
                        make_specular_map_filename(model_filename)};
            Shader shader{model, light_dir};
            for (int i = 0; i < model.nfaces(); i++)
            {
                for (int j = 0; j < 3; j++)
                {
                    shader.vertex(i, j);
                }
                triangle(shader.varying_tri, shader, frame, zbuffer.data());
            }
        }
        frame.flip_vertically(); // to place the origin in the bottom left
                                 // corner of the image
        frame.write_tga_file("framebuffer.tga");

        return 0;
    }
    catch (std::exception& e)
    {
        std::cerr << "An exception was thrown: " << e.what() << '\n';
    }
    catch (...)
    {
        std::cerr << "An unknown exception was thrown\n";
    }
    return -1;
}
