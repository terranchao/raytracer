
#include <cmath>
#include <cstdint>

#include "sphere.hpp"
#include "vec3.hpp"

Sphere::Sphere(
    const Vec3& _center, const float& _radius, const uint32_t& _color
) : center(_center), radius(_radius), color(_color)
{
    r2 = (radius*radius);
}

bool Sphere::is_intersected_by(const Vec3& origin, const Vec3& dir)
{
    /* Geometric solution */
    const Vec3 L = center - origin;
    const float tca = (L*dir);
    if (tca < 0) return false;
    const float d2 = (L*L)-(tca*tca);
    if (d2 > r2) return false;
    const float thc = sqrt(r2-d2);
    const float t0 = tca - thc;
    const float t1 = tca + thc;
    (void)t0;
    (void)t1;
    return true;
}
