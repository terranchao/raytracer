#ifndef VEC3_HPP
#define VEC3_HPP

struct Vec3
{
    float x = 0.f, y = 0.f, z = 0.f;
    Vec3(const float& _x, const float& _y, const float& _z);
    Vec3 operator-(const Vec3& v) const;
    float operator*(const Vec3& v) const;
    Vec3 normalized() const;
};

#endif // VEC3_HPP
