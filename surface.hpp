#ifndef SURFACE_HPP
#define SURFACE_HPP

#include <cstdint>

struct Surface
{
    uint32_t base_color = 0xff000000; // SDL_PIXELFORMAT_ARGB8888
    Surface(const uint32_t& _base_color);
    uint32_t get_color(const float& scale) const;
};

#endif // SURFACE_HPP
