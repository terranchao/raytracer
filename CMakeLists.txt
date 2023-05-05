cmake_minimum_required(VERSION 3.22.1)

project(raytracer CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_definitions(DEBUG)
endif()

file(GLOB SOURCES *.cpp)
add_executable(${PROJECT_NAME} ${SOURCES})

find_package(OpenMP COMPONENTS CXX)
if(OPENMP_FOUND)
    add_compile_definitions(OPENMP)
endif()

target_compile_options(${PROJECT_NAME}
PRIVATE
    -fstack-protector-all
    -Wall
    -Werror
    -Wextra
    -Wpedantic
)

target_link_libraries(${PROJECT_NAME}
PRIVATE
    -lSDL2
    -lncurses
    $<$<BOOL:${OpenMP_CXX_FOUND}>:OpenMP::OpenMP_CXX>
)
