#ifndef SPHERE_HPP
#define SPHERE_HPP

#include "surface.hpp"
#include "vec3.hpp"

struct Sphere
{
private:
    float r2 = 0.f;
public:
    Vec3 center;
    float radius = 0.f;
    Surface surface;
    Sphere(const Vec3& _center, const float& _radius, const Surface& _surface);
    bool is_intersected_by(
        const Vec3& origin, const Vec3& dir, float& distance
    ) const;
};

#endif // SPHERE_HPP
