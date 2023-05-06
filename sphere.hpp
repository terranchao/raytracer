#ifndef SPHERE_HPP
#define SPHERE_HPP

#include <cstdint>

#include "material.hpp"
#include "vec3.hpp"

#define SET_ALPHA 0xff000000

struct Sphere
{
private:
    float r2 = 0.f;
public:
    Vec3 center;
    float radius = 0.f;
    uint32_t base_color = SET_ALPHA; // SDL_PIXELFORMAT_ARGB8888
    Material material;
    Sphere(
        const Vec3& _center,
        const float& _radius,
        const uint32_t& _base_color,
        const MaterialType& _material_type
    );
    bool is_intersected_by(
        const Vec3& origin, const Vec3& dir, float& distance
    ) const;
};

#endif // SPHERE_HPP
