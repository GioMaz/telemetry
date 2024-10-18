# Telemetry

Recruiting project for the e-agle telemetry team.

### Prerequisites

- `git` and a [GitHub](https://github.com) account
- C/C++ toolchain, with CMake
- OpenGL
- GLEW
- GLFW3

For Debian / Ubuntu you can use:

```bash
sudo apt install build-essential cmake libglfw3-dev libglew-dev mesa-utils
```

### Building

The build system that we use is cmake.

```bash
mkdir -p build
cmake ..
make -j$(nproc)
```

This will build the executable that will be located in `./bin` directory.
