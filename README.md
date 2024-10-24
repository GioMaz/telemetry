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

When first starting the application the admin is registered with the credentials:
- username: admin
- password: admin

> The admin user gets recreated on every application launch

The capabilities of the Admin user are:
- Accessing all the plots
- Disabling some plots for all users
- Create and modify existing users
- Modifying his own profile

Instead, the capabilities of the Viewer users are:
- Accessing the designated plots
- Modifying his own profile
