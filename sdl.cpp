

#include <SDL2/SDL.h>
#include <cstdint>
#include <ncurses.h>
#include <stdexcept>
#include <string>

#include "sdl.hpp"

class SDLError : public std::runtime_error
{
public:
    SDLError(const std::string& what = "") : std::runtime_error(what) {}
};

SDL::SDL(const size_t w, const size_t h)
{
    width_in_bytes = w * sizeof(uint32_t);
    framebuffer = (uint32_t*)malloc(h*width_in_bytes);

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        throw SDLError("Unable to initialize");
    }

    window = SDL_CreateWindow(
        "raytracer",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        w,
        h,
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
        w,
        h
    );
    if (!texture)
    {
        throw SDLError("Unable to create texture");
    }

    /* ncurses */
    initscr();
    curs_set(0);
}

SDL::~SDL()
{
    /* ncurses */
    endwin();

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

bool SDL::quit()
{
    SDL_Event e;
    while (SDL_PollEvent(&e))
    {
        if (
            (e.type == SDL_QUIT) ||
            ((e.type == SDL_KEYUP) && (e.key.keysym.sym == SDLK_ESCAPE))
        )
        {
            return true;
        }
    }
    return false;
}

void SDL::render()
{
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

void SDL::start_fps_count()
{
    fps_counter = SDL_GetPerformanceCounter();
}

void SDL::show_fps_count()
{
    static const int fps_row = getmaxy(stdscr)-1;
    static const float counts_per_second = (float)SDL_GetPerformanceFrequency();
    mvprintw(
        fps_row, 1, "FPS: %.0f",
        counts_per_second/(SDL_GetPerformanceCounter()-fps_counter)
    );
    refresh();
}
