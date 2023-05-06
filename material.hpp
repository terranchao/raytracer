#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include <tuple>
#include <unordered_map>

enum class MaterialType
{
    Matte,
    Mirror,
};

struct Material
{
    float diffuse_constant = 0.f;
    float specular_constant = 0.f;
    float shininess_constant = 0.f;
    float refractive_index = 0.f;
    Material(const MaterialType& _material_type);
};

extern std::unordered_map<
    const MaterialType, const std::tuple<float, float, float, float>
> g_materials;

#endif // MATERIAL_HPP
