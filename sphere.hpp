#ifndef SPHERE_HPP
#define SPHERE_HPP

#include <cstdint>

#include "vec3.hpp"

struct Sphere
{
private:
    float r2 = 0;
public:
    Vec3 center;
    float radius = 0;
    uint32_t color = 0xff000000; // Pixel format: ARGB
    Sphere(const Vec3& _center, const float& _radius, const uint32_t& _color);
    bool is_intersected_by(const Vec3& origin, const Vec3& dir);
};

#endif // SPHERE_HPP
