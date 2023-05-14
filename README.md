
# A ray tracing program

This project is a ray tracing demonstration written in C++. A 3D environment
is virtualized and rendered, and the result is displayed in a window using the
[SDL development library](https://www.libsdl.org/).

Click [here](https://github.com/terranchao/raytracer/wiki/Progress-slideshow)
for a slideshow of development screenshots.

![14](https://github.com/terranchao/raytracer/assets/5587757/cfba7648-291c-471f-820d-0228eab53799)

## Features

### 3D visuals

- Sphere subjects
    - with matte finish
    - with mirror finish
    - made of glass (TODO)
- Lighting
    - Diffuse
    - Specular
- Shadows
- Reflections
- Refractions (TODO)

### CPU-only

As an exercise, all graphics computations (aside from SDL rendering) are run on
the CPU instead of the GPU. No external graphics libraries are used.

Although efforts have been made to improve performance, it is a secondary
priority of this exercise. We have already excluded the benefits of hardware
acceleration. The main goal here is to provide a simple demonstration of how ray
tracing actually works.

Click [here](https://github.com/terranchao/raytracer/blob/main/scene.cpp#L188)
to view the the start of the code for the graphics computation logic.
`Scene::write_frame()` generates a camera ray for each pixel in the display, and
`cast()` follows, or "traces", each ray in order to compute the color of its
corresponding pixel.

[OpenMP](https://www.openmp.org/) is leveraged to parallelize the ray casting.

### FPS counter

A frame per second count is written to the terminal screen in real time using
the [ncurses](https://en.wikipedia.org/wiki/Ncurses) library. Checking this
value is the simplest way to estimate performance. Observed values may vary
across different runtime machines/environments.

## Run it yourself

**Note:** This project was developed in
[WSL (Windows Subsystem for Linux)](https://en.wikipedia.org/wiki/Windows_Subsystem_for_Linux)
Version 2. WSL users must make sure their own version of WSL is new enough to
support WSLg, which makes it possible for a SDL window to launch from within
WSL.

1. **Install dependencies**

    On Debian systems
    ```bash
    sudo apt update
    sudo apt install build-essential cmake libncurses-dev libsdl2-dev
    ```

2. **Download**

    ```bash
    git clone https://github.com/terranchao/raytracer.git
    ```

3. **Build**

    CMake is used to build this project.
    ```bash
    cd raytracer
    cmake -B build
    cmake --build build
    ```

4. **Run**

    ```
    ./build/raytracer
    ```

    **For WSL users**: There is a known WSLg-SDL compatibility
    [issue](https://github.com/microsoft/wslg/issues/715) that causes a
    segmentation fault. If a segmentation fault occurs, try running the
    following and retrying. This must be done once per shell instance.
    ```bash
    $ export LIBGL_ALWAYS_SOFTWARE=1
    ```

## References

Thank you to these pages for the inspiration.

- https://github.com/ssloy/tinyraytracer/wiki/Part-1:-understandable-raytracing
- https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-sphere-intersection.html
- https://www.scratchapixel.com/lessons/3d-basic-rendering/ray-tracing-generating-camera-rays/generating-camera-rays.html
- https://en.wikipedia.org/wiki/Phong_reflection_model
