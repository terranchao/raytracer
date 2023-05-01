
#include <exception>
#include <iostream>

#include "sdl.hpp"

int main()
{
    try
    {
        SDL sdl;
        sdl.loop();
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    return 0;
}
