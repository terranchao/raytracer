
#include <cassert>
#include <cmath>
#include <cstdint>
#include <limits>
#include <tuple>
#include <unordered_map>

#include "light.hpp"
#include "material.hpp"
#include "scene.hpp"
#include "sdl.hpp"
#include "sphere.hpp"
#include "vec3.hpp"

std::unordered_map<
    const MaterialType, const std::tuple<float, float, float, float>
> g_materials = {
    //                      Diffuse Specular Shininess Refractive
    {MaterialType::Matte,  {    1.f,     0.f,      0.f,       0.f}},
    {MaterialType::Mirror, {   1.2f,     6.f,     61.f,       0.f}},
};

static const Sphere g_spheres[] = {
//   Center                     Radius  Color       MaterialType
    {{ -20.f, -90.f, -480.f},    110.f, 0xffcc4488, MaterialType::Matte},
    {{ 130.f,   0.f, -460.f},     60.f, 0xff4488cc, MaterialType::Mirror},
    {{-200.f,   0.f, -550.f},     90.f, 0xff88cc44, MaterialType::Matte},
    {{ -30.f,  70.f, -510.f},     30.f, 0xff8484cc, MaterialType::Mirror},
};
static const Light g_lights[] = {
//      Position                 Intensity
    {   {   0.f, 300.f,  -50.f},      1.2f  },
    {   { 200.f, 140.f, -450.f},       .9f  },
};

static const Vec3 CAMERA{0.f, 0.f, 0.f};
static constexpr float FOV = (M_PI/3.f);
extern constexpr size_t SCREEN_WIDTH = 1024;
extern constexpr size_t SCREEN_HEIGHT = 768;
static constexpr size_t NUM_PIXELS = (SCREEN_WIDTH*SCREEN_HEIGHT);
static constexpr size_t SCREEN_WIDTH_HALF = (SCREEN_WIDTH/2);
static constexpr size_t SCREEN_HEIGHT_HALF = (SCREEN_HEIGHT/2);
static constexpr float RAY_X_OFFSET = (0.5f-SCREEN_WIDTH_HALF);
static constexpr float RAY_Y_OFFSET = (SCREEN_HEIGHT_HALF-0.5f);
static constexpr float RAY_Z = -(float)SCREEN_HEIGHT_HALF/tan(FOV/2);
static constexpr uint32_t BACKGROUND_COLOR = 0xff222211;
static constexpr int MAX_REFLECTIONS = 2;

static uint32_t blend(
    const uint32_t& color1,
    const uint32_t& color2,
    const float& percent_of_color1)
{
    assert((percent_of_color1 >= 0.f) && (percent_of_color1 <= 1.f));
    const float percent_of_color2 = (1.f-percent_of_color1);
    const uint32_t r1 = (color1 & 0x00ff0000) >> 16;
    const uint32_t g1 = (color1 & 0x0000ff00) >> 8;
    const uint32_t b1 = (color1 & 0x000000ff);
    const uint32_t r2 = (color2 & 0x00ff0000) >> 16;
    const uint32_t g2 = (color2 & 0x0000ff00) >> 8;
    const uint32_t b2 = (color2 & 0x000000ff);
    const float r = (percent_of_color1*r1) + (percent_of_color2*r2);
    const float g = (percent_of_color1*g1) + (percent_of_color2*g2);
    const float b = (percent_of_color1*b1) + (percent_of_color2*b2);
    return (SET_ALPHA|((uint32_t)r<<16)|((uint32_t)g<<8)|(uint32_t)b);
}

static uint32_t add_lighting(
    const uint32_t& color,
    const float& diffuse_constant,
    const float& diffuse_intensity,
    const float& specular_constant,
    const float& specular_intensity
)
{
    uint32_t r = (color & 0x00ff0000) >> 16;
    uint32_t g = (color & 0x0000ff00) >> 8;
    uint32_t b = (color & 0x000000ff);
    const float scale = (
        (diffuse_constant*diffuse_intensity) +
        (specular_constant*specular_intensity)
    );
    r = (uint32_t)std::min(255.f, (float)r*scale);
    g = (uint32_t)std::min(255.f, (float)g*scale);
    b = (uint32_t)std::min(255.f, (float)b*scale);
    return (SET_ALPHA|(r<<16)|(g<<8)|b);
}

static uint32_t cast(const Vec3& origin, const Vec3& dir, int depth = 0)
{
    if (depth > MAX_REFLECTIONS)
    {
        return BACKGROUND_COLOR;
    }

    /* Hit nearest sphere (or nothing) */
    float min_distance = std::numeric_limits<float>::max();
    const Sphere *nearest = nullptr;
    for (const auto& sphere : g_spheres)
    {
        float distance;
        if (!sphere.is_intersected_by(origin, dir, distance)) continue;
        if (distance < min_distance)
        {
            min_distance = distance;
            nearest = &sphere;
        }
    }
    if (!nearest)
    {
        return BACKGROUND_COLOR;
    }

    /* Phong reflection model: Compute lighting */
    const Vec3 intersect = origin + (dir*min_distance);
    const Vec3 normal = (intersect-nearest->center).normalized();
    float diffuse_intensity = 0.f, specular_intensity = 0.f;
    for (const auto& light : g_lights)
    {
        const Vec3 light_dir = (light.position-intersect).normalized();

        // Shadows - Ignore light computation if there is an object in the way.
        bool shadow = false;
        for (const auto& sphere : g_spheres)
        {
            if (nearest == &sphere) continue;
            float unused;
            if (sphere.is_intersected_by(intersect, light_dir, unused))
            {
                shadow = true;
                break;
            }
        }
        if (shadow) continue;

        // Diffuse
        const float diffuse_magnitude = (light_dir*normal);
        if (diffuse_magnitude > 0.f)
        {
            diffuse_intensity += (light.intensity*diffuse_magnitude);
        }

        // Specular
        if (nearest->material.specular_constant > 0.f)
        {
            const Vec3 light_reflect_dir = light_dir.reflected(normal);
            const float specular_magnitude = powf(
                light_reflect_dir*dir,
                nearest->material.shininess_constant
            );
            if (specular_magnitude > 0.f)
            {
                specular_intensity += (light.intensity*specular_magnitude);
            }
        }
    }

    /* Compute color */
    uint32_t color = nearest->base_color;
    if (nearest->material.specular_constant > 0.f)
    {
        // Reflections
        color = blend(
            color,
            cast(
                intersect, dir.reflected(normal), depth+1
            ),
            0.8f
        );
    }
    return add_lighting(
        color,
        nearest->material.diffuse_constant,
        diffuse_intensity,
        nearest->material.specular_constant,
        specular_intensity
    );
}

static inline size_t calculate_row_offset(
    const size_t& row, const size_t& screen_width
)
{
    return (screen_width == 1024) ? (row<<10) : (row*screen_width);
}

void Scene::write_frame()
{
    /* Generate camera rays */
#ifdef OPENMP
#pragma omp parallel for
#endif
    for (size_t row = 0; row < SCREEN_HEIGHT; row++)
    {
        const size_t row_offset = calculate_row_offset(row, SCREEN_WIDTH);
        for (size_t col = 0; col < SCREEN_WIDTH; col++)
        {
            sdl.framebuffer[row_offset+col] = cast(
                CAMERA,
                Vec3{(col+RAY_X_OFFSET), (RAY_Y_OFFSET-row), RAY_Z}.normalized()
            );
        }
    }
}

void Scene::run()
{
    while (true)
    {
        sdl.start_fps_count();
        if (sdl.quit()) break;
        write_frame();
        sdl.render();
        sdl.show_fps_count();
    }
}
