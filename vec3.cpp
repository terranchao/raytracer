
#include <cmath>

#include "vec3.hpp"

Vec3::Vec3(const float& _x, const float& _y, const float& _z)
    : x(_x), y(_y), z(_z) {}

Vec3 Vec3::operator-() const
{
    return {-x, -y, -z};
}

Vec3 Vec3::operator-(const Vec3& v) const
{
    return {(x-v.x), (y-v.y), (z-v.z)};
}

Vec3 Vec3::operator+(const Vec3& v) const
{
    return {(x+v.x), (y+v.y), (z+v.z)};
}

float Vec3::operator*(const Vec3& v) const
{
    return (x*v.x) + (y*v.y) + (z*v.z);
}

Vec3 Vec3::operator*(const float& s) const
{
    return {(x*s), (y*s), (z*s)};
}

Vec3 Vec3::normalized() const
{
    const float L = (1.f/sqrt((x*x)+(y*y)+(z*z)));
    return {(L*x), (L*y), (L*z)};
}
