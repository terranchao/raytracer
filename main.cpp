
#include <exception>
#include <iostream>

#include "scene.hpp"

int main()
{
    try
    {
        Scene scene;
        scene.run();
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}
