#ifndef SURFACE_HPP
#define SURFACE_HPP

#include <cstdint>

#include "material.hpp"

#define SET_ALPHA 0xff000000

struct Surface
{
    uint32_t base_color = SET_ALPHA; // SDL_PIXELFORMAT_ARGB8888
    Material material;
    Surface(const uint32_t& _base_color, const MaterialType& _material_type);
    uint32_t get_color(
        const uint32_t& blend_color,
        const float& diffuse_intensity,
        const float& specular_intensity
    ) const;
};

#endif // SURFACE_HPP
