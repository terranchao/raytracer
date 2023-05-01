#ifndef SCENE_HPP
#define SCENE_HPP

#include "sdl.hpp"

#define SCREEN_WIDTH   1366
#define SCREEN_HEIGHT  768

class Scene
{
private:
    SDL sdl{SCREEN_WIDTH, SCREEN_HEIGHT};
public:
    Scene();
    void run();
};

#endif // SCENE_HPP
