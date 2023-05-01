
#include "scene.hpp"
#include "sdl.hpp"

Scene::Scene()
{
}

void Scene::run()
{
    while (true)
    {
        if (sdl.quit()) return;
        sdl.render();
    }
}
