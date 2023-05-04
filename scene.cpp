
#ifdef DEBUG
#include <chrono>
#include <iostream>
#endif
#include <cmath>
#include <cstdint>
#include <limits>

#include "light.hpp"
#include "scene.hpp"
#include "sdl.hpp"
#include "sphere.hpp"
#include "surface.hpp"
#include "vec3.hpp"

static const Sphere g_spheres[] = {
//      Center                      Radius      Surface
    {   {   0.f, -60.f, -400.f},    60.f,       {0xffcc4488}    },
    {   {-160.f, 120.f, -460.f},    80.f,       {0xff88cc44}    },
    {   { 100.f,  20.f, -480.f},    90.f,       {0xff4488cc}    },
};
static const Light g_lights[] = {
//      Position                Intensity
    {   {   0.f, 200.f, 0.f},   1.2f        },
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

uint32_t Scene::cast(const Vec3& dir)
{
    /* Hit nearest sphere (or nothing) */
    float min_distance = std::numeric_limits<float>::max();
    const Sphere *nearest = nullptr;
    for (const auto& sphere : g_spheres)
    {
        float distance;
        if (!sphere.is_intersected_by(CAMERA, dir, distance)) continue;
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

    /* Compute color */
    const Vec3 intersect = CAMERA + (dir*min_distance);
    const Vec3 normal = (intersect-nearest->center).normalized();
    float diffuse_intensity = 0.f;
    for (const auto& light : g_lights)
    {
        const Vec3 light_dir = (light.position-intersect).normalized();
        const float diffuse_magnitude = (light_dir*normal);
        if (diffuse_magnitude > 0.f)
        {
            diffuse_intensity += (light.intensity*diffuse_magnitude);
        }
    }
    return nearest->surface.get_color(diffuse_intensity);
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
#ifdef DEBUG
    const auto start = std::chrono::steady_clock::now();
#endif
#ifdef OPENMP
    #pragma omp parallel for
#endif
    for (size_t row = 0; row < SCREEN_HEIGHT; row++)
    {
        const size_t row_offset = calculate_row_offset(row, SCREEN_WIDTH);
        for (size_t col = 0; col < SCREEN_WIDTH; col++)
        {
            sdl.framebuffer[row_offset+col] = cast(
                Vec3{(col+RAY_X_OFFSET), (RAY_Y_OFFSET-row), RAY_Z}.normalized()
            );
        }
    }
#ifdef DEBUG
#ifdef OPENMP
    #pragma omp barrier
#endif
    std::cerr << std::chrono::duration_cast<std::chrono::nanoseconds>(
        std::chrono::steady_clock::now() - start
    ).count() << " ns" << std::endl;
#endif
}

void Scene::run()
{
    while (true)
    {
        if (sdl.quit()) return;
        write_frame();
        sdl.render();
    }
}
