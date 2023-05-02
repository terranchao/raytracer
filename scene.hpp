#ifndef SCENE_HPP
#define SCENE_HPP

#include <cstdint>

#include "sdl.hpp"
#include "vec3.hpp"

extern const size_t SCREEN_WIDTH;
extern const size_t SCREEN_HEIGHT;

class Scene
{
private:
    SDL sdl{SCREEN_WIDTH, SCREEN_HEIGHT};
    uint32_t cast(const Vec3& dir);
    void write_frame();
public:
    void run();
};

#endif // SCENE_HPP
