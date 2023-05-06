
#include <tuple>
#include <unordered_map>

#include "material.hpp"

Material::Material(const MaterialType& _material_type)
{
    const auto m = g_materials[_material_type];
    diffuse_constant   = std::get<0>(m);
    specular_constant  = std::get<1>(m);
    shininess_constant = std::get<2>(m);
    refractive_index   = std::get<3>(m);
}
