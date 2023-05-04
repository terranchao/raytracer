#ifndef LIGHT_HPP
#define LIGHT_HPP

#include <algorithm>

#include "vec3.hpp"

struct Light
{
    Vec3 position;
    float intensity = 0.f;
    Light(const Vec3& _position, const float& _intensity)
        : position(_position), intensity(std::max(0.f, _intensity)) {}
};

#endif // LIGHT_HPP
