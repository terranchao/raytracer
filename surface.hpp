#ifndef SURFACE_HPP
#define SURFACE_HPP

#include <cstdint>

struct Surface
{
    uint32_t base_color = 0xff000000; // SDL_PIXELFORMAT_ARGB8888
    float diffuse_constant = 0.f;
    float specular_constant = 0.f;
    float shininess_constant = 0.f;
    Surface(
        const uint32_t& _base_color,
        const float& _diffuse_constant,
        const float& _specular_constant,
        const float& _shininess_constant
    );
    uint32_t get_color(const float& diffuse_intensity) const;
};

#endif // SURFACE_HPP
