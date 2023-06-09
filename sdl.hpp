#ifndef SDL_HPP
#define SDL_HPP

#include <SDL2/SDL.h>
#include <cstdint>

class SDL
{
private:
    SDL_Window *window = nullptr;
    SDL_Renderer *renderer = nullptr;
    SDL_Texture *texture = nullptr;
    size_t width_in_bytes = 0;
    uint64_t fps_counter = 0;
public:
    uint32_t *framebuffer = nullptr;
    SDL(const size_t w, const size_t h);
    ~SDL();
    bool quit();
    void render();
    void start_fps_count();
    void show_fps_count();
};

#endif // SDL_HPP
