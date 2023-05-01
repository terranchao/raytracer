

#include <SDL2/SDL.h>
#include <cstdint>
#include <stdexcept>
#include <string>

#include "sdl.hpp"

static constexpr size_t DISPLAY_WIDTH = 1366;
static constexpr size_t DISPLAY_HEIGHT = 768;

class SDLError : public std::runtime_error
{
public:
    SDLError(const std::string& what = "") : std::runtime_error(what) {}
};

SDL::SDL()
{
    width_in_bytes = DISPLAY_WIDTH * sizeof(uint32_t);
    framebuffer = (uint32_t*)malloc(DISPLAY_HEIGHT*width_in_bytes);

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        throw SDLError("Unable to initialize");
    }

    window = SDL_CreateWindow(
        "raytracer",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        DISPLAY_WIDTH,
        DISPLAY_HEIGHT,
        SDL_WINDOW_SHOWN
    );
    if (!window)
    {
        throw SDLError("Unable to create window");
    }

    renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED
    );
    if (!renderer)
    {
        throw SDLError("Unable to create renderer");
    }

    texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_ARGB8888 , // fast
        SDL_TEXTUREACCESS_STREAMING,
        DISPLAY_WIDTH,
        DISPLAY_HEIGHT
    );
    if (!texture)
    {
        throw SDLError("Unable to create texture");
    }
}

SDL::~SDL()
{
    if (texture)
    {
        SDL_DestroyTexture(texture);
        texture = nullptr;
    }
    if (renderer)
    {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }
    if (window)
    {
        SDL_DestroyWindow(window);
        window = nullptr;
    }
    if (framebuffer)
    {
        free(framebuffer);
        framebuffer = nullptr;
    }
    SDL_Quit();
}

void SDL::loop()
{
    while (true)
    {
        /* Events */
        SDL_Event e;
        while (SDL_PollEvent(&e))
        {
            if (
                (e.type == SDL_QUIT) ||
                ((e.type == SDL_KEYUP) && (e.key.keysym.sym == SDLK_ESCAPE))
            )
            {
                return;
            }
        }

        /* Render */
        SDL_UpdateTexture(
            texture,
            nullptr,
            framebuffer,
            width_in_bytes
        );
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, nullptr, nullptr);
        SDL_RenderPresent(renderer);
    }
}
