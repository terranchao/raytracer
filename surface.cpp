
#include <algorithm>
#include <cstdint>

#include "surface.hpp"

Surface::Surface(const uint32_t& _base_color) : base_color(_base_color) {}

uint32_t Surface::get_color(const float& scale) const
{
    uint32_t r = (base_color & 0x00ff0000) >> 16;
    uint32_t g = (base_color & 0x0000ff00) >> 8;
    uint32_t b = (base_color & 0x000000ff);

    r = (uint32_t)std::min(255.f, (float)r*scale);
    g = (uint32_t)std::min(255.f, (float)g*scale);
    b = (uint32_t)std::min(255.f, (float)b*scale);
    
    return (0xff000000|(r<<16)|(g<<8)|b);
}
