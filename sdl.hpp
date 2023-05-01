#ifndef SDL_HPP
#define SDL_HPP

#include <SDL2/SDL.h>
#include <cstdint>

class SDL
{
private:
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *texture;
    uint32_t *framebuffer;
    size_t width_in_bytes;
public:
    SDL();
    ~SDL();
    void loop();
};

#endif // SDL_HPP
