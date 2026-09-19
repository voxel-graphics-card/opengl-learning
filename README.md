# opengl-learning

A small OpenGL 4.3 core-profile learning program built with GLFW and GLAD. It
initializes a window, sets up a vertex array with an index buffer, loads two
shaders, and draws a single colored rectangle.

## Requirements

- A C++17 compiler (g++, clang, or MSVC)
- A GPU and driver supporting OpenGL 4.3 core profile
- GLFW 3.x
- GLAD generated for OpenGL 4.3 core with a loader function
- A `dependencies/include` folder at the project root (see below)

## Dependencies setup

### GLAD

Generate the loader for your platform at the [GLAD web service](https://glad.dav1d.de):

- Language: C/C++
- API: `gl` version `4.3`, profile `Core`, `Generate a loader`
- Copy the generated `include/glad/gl.h` and `include/KHR/khrplatform.h` into
  `dependencies/include/` so the layout matches the includes used in the source
  (for example `dependencies/include/glad/gl.h`).

### GLFW

- Linux (Debian/Ubuntu):
  ```
  sudo apt install libglfw3-dev
  ```
  Other distros provide the equivalent `glfw` development package.
- Windows: place the GLFW headers under `dependencies/include/GLFW/` and the
  library under `dependencies/lib/`, or install GLFW via vcpkg.

## Build

Run the commands from the project root so the relative shader paths resolve.

### Linux

```
g++ -std=c++17 -I src -I dependencies/include \
    src/main.cpp \
    src/core/window.cpp \
    src/core/debug.cpp \
    src/renderer/gl_helper.cpp \
    src/renderer/shader.cpp \
    src/renderer/index_buffer.cpp \
    src/renderer/vertex_array.cpp \
    src/renderer/vertex_buffer.cpp \
    $(pkg-config --cflags --libs glfw3) -lGL \
    -o opengl-learning

./opengl-learning
```

### Windows (MinGW)

```
g++ -std=c++17 -I src -I dependencies/include ^
    src\main.cpp ^
    src\core\window.cpp ^
    src\core\debug.cpp ^
    src\renderer\gl_helper.cpp ^
    src\renderer\shader.cpp ^
    src\renderer\index_buffer.cpp ^
    src\renderer\vertex_array.cpp ^
    src\renderer\vertex_buffer.cpp ^
    -Idependencies\include -lglfw3 -lopengl32 ^
    -o opengl-learning.exe

opengl-learning.exe
```

For MSVC, keep your local `build.bat` in sync with the source list above. The
include path needs `dependencies\include` so `<glad/gl.h>` and
`<GLFW/glfw3.h>` resolve.

## Shaders

The program loads `shaders/vertex.glsl` and `shaders/fragment.glsl` at runtime,
relative to the working directory. These files are gitignored, so create them
yourself:

`shaders/vertex.glsl`:

```
#version 430 core

layout (location = 0) in vec2 aPos;

void main()
{
    gl_Position = vec4(aPos, 0.0, 1.0);
}
```

`shaders/fragment.glsl`:

```
#version 430 core

uniform vec4 ourColor;

out vec4 FragColor;

void main()
{
    FragColor = ourColor;
}
```

These match what `src/main.cpp` expects: attribute index 0 for `aPos`, and the
`ourColor` uniform set via `shader.setVec4(...)`.

## Tests

```
g++ -std=c++17 tests/test.cpp -o /tmp/gl_test
/tmp/gl_test
```

## Project structure

```
.
|-- src/
|   |-- main.cpp                    entry point and render loop
|   |-- core/
|   |   |-- window.hpp/.cpp         GLFW window and OpenGL context setup
|   |   `-- debug.hpp/.cpp          GL error handler and GL_CALL macro
|   `-- renderer/
|       |-- buffer_layout.hpp       vertex attribute layout definition
|       |-- gl_helper.hpp/.cpp      low-level GL abstractions
|       |-- index_buffer.hpp/.cpp   element buffer wrapper
|       |-- shader.hpp/.cpp         shader source loading and uniform helpers
|       |-- vertex_array.hpp/.cpp   vertex array object wrapper
|       `-- vertex_buffer.hpp/.cpp  vertex buffer wrapper
|-- tests/
|   `-- test.cpp
|-- .gitignore
|-- LICENSE
`-- README.md
```

## Debugging

`src/core/debug.hpp` enables OpenGL debug output and installs a callback that
logs driver messages. Wrap draw calls in `GL_CALL(...)` to have errors reported
with the file and line at the call site. High-severity errors trap in a
debugger; building with `NDEBUG` disables the checks.

## License and credit

MIT, see [LICENSE](LICENSE). This project started as a fork of
[amlan-sinha07/opengl-learning](https://github.com/amlan-sinha07/opengl-learning).