#ifndef SURFACE_HPP
#define SURFACE_HPP

#include <cstdint>

#define SET_ALPHA 0xff000000

struct Surface
{
    uint32_t base_color = SET_ALPHA; // SDL_PIXELFORMAT_ARGB8888
    float diffuse_constant = 0.f;
    float specular_constant = 0.f;
    float shininess_constant = 0.f;
    Surface(
        const uint32_t& _base_color,
        const float& _diffuse_constant,
        const float& _specular_constant,
        const float& _shininess_constant
    );
    uint32_t get_color(
        const uint32_t& blend_color,
        const float& diffuse_intensity,
        const float& specular_intensity
    ) const;
};

#endif // SURFACE_HPP
