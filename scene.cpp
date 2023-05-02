
#include <cmath>
#include <cstdint>
#include <limits>

#include "scene.hpp"
#include "sdl.hpp"
#include "sphere.hpp"
#include "vec3.hpp"

static Sphere g_spheres[] = {
//      Center                      Radius      Color
    {   {   0.f, -60.f, -200.f},    60.f,       0xffcc4488    },
    {   {-160.f, 120.f, -260.f},    80.f,       0xff88cc44    },
    {   { 100.f,  20.f, -280.f},    90.f,       0xff4488cc    },
};
static constexpr size_t NUM_SPHERES = sizeof(g_spheres)/sizeof(Sphere);

static const Vec3 CAMERA{0.f, 0.f, 0.f};
static constexpr float FOV = (M_PI/2.f);

extern constexpr size_t SCREEN_WIDTH = 1366;
extern constexpr size_t SCREEN_HEIGHT = 768;
static constexpr size_t NUM_PIXELS = (SCREEN_WIDTH*SCREEN_HEIGHT);
static constexpr size_t SCREEN_WIDTH_HALF = (SCREEN_WIDTH/2);
static constexpr size_t SCREEN_HEIGHT_HALF = (SCREEN_HEIGHT/2);
static constexpr float ASPECT_RATIO = (SCREEN_WIDTH/SCREEN_HEIGHT);
static constexpr uint32_t BACKGROUND_COLOR = 0xff000000;

uint32_t Scene::cast(const Vec3& dir)
{
    /* Hit closest sphere or nothing */
    int nearest = -1;
    float min_distance = std::numeric_limits<float>::max();
    for (size_t i = 0; i < NUM_SPHERES; i++)
    {
        if (!g_spheres[i].is_intersected_by(CAMERA, dir)) continue;

        const float distance =
            CAMERA.distance_to(g_spheres[i].center) - g_spheres[i].radius;
        if (distance < min_distance)
        {
            min_distance = distance;
            nearest = (int)i;
        }
    }
    if (nearest > -1)
    {
        return g_spheres[nearest].color;
    }
    else
    {
        return BACKGROUND_COLOR;
    }
}

void Scene::write_frame()
{
    /* Generate camera rays */
    for (size_t i = 0; i < NUM_PIXELS; i++)
    {
        const Vec3 ray{
            ((i%SCREEN_WIDTH)+0.5f)-SCREEN_WIDTH_HALF,
            SCREEN_HEIGHT_HALF-((i/SCREEN_WIDTH)+0.5f),
            -(float)SCREEN_HEIGHT_HALF/tan(FOV/2.f)
        };
        sdl.framebuffer[i] = cast(ray.normalized());
    }
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
