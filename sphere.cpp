
#include <algorithm>
#include <cmath>

#include "sphere.hpp"
#include "vec3.hpp"

Sphere::Sphere(
    const Vec3& _center, const float& _radius, const Surface& _surface
) : center(_center), radius(std::max(0.f, _radius)), surface(_surface)
{
    r2 = (radius*radius);
}

bool Sphere::is_intersected_by(
    const Vec3& origin, const Vec3& dir, float& distance
) const
{
    /* Geometric solution */
    const Vec3 L = center - origin;
    const float tca = (L*dir);
    if (tca < 0.f) return false;
    const float d2 = (L*L)-(tca*tca);
    if (d2 > r2) return false;
    const float thc = sqrt(r2-d2);
    const float t0 = (tca-thc);
    const float t1 = (tca+thc);
    if (t0 > 0.f)
    {
        distance = t0;
        return true;
    }
    else if (t1 > 0.f)
    {
        distance = t1;
        return true;
    }
    else
    {
        distance = t0;
        return false;
    }
}
