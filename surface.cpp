
#include <algorithm>
#include <cstdint>

#include "surface.hpp"

Surface::Surface(
    const uint32_t& _base_color, const MaterialType& _material_type
) : base_color(_base_color), material(_material_type) {}

uint32_t Surface::get_color(
    const uint32_t& blend_color,
    const float& diffuse_intensity,
    const float& specular_intensity
) const
{
    uint32_t r = (base_color & 0x00ff0000) >> 16;
    uint32_t g = (base_color & 0x0000ff00) >> 8;
    uint32_t b = (base_color & 0x000000ff);
    if (blend_color != base_color)
    {
        const uint32_t _r = (blend_color & 0x00ff0000) >> 16;
        const uint32_t _g = (blend_color & 0x0000ff00) >> 8;
        const uint32_t _b = (blend_color & 0x000000ff);
        r = (r+_r)/2;
        g = (g+_g)/2;
        b = (b+_b)/2;
    }
    const float scale = (
        (material.diffuse_constant*diffuse_intensity) +
        (material.specular_constant*specular_intensity)
    );
    r = (uint32_t)std::min(255.f, (float)r*scale);
    g = (uint32_t)std::min(255.f, (float)g*scale);
    b = (uint32_t)std::min(255.f, (float)b*scale);
    return (SET_ALPHA|(r<<16)|(g<<8)|b);
}
