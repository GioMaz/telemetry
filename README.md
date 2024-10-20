# Telemetry

Recruiting project for the e-agle telemetry team.

### Prerequisites

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

### Usage

When first starting the application there are 2 users registered in the system

| email       | pasword | role   |
|-------------|---------|--------|
| a@gmail.com | a       | Admin  |
| v@gmail.com | v       | Viewer |

- The Admin user can access all of the plots, disable plots for viewer users and create and modify existing users
- The Viewer user can access designated plots and modify his own profile
