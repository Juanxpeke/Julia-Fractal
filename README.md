# Julia-Fractal

Application for showing various types of fractals from Julia Sets (http://paulbourke.net/fractals/juliaset/). It uses OpenGL for the graphics, and the entire logic for the fractals is inside the fragment shaders.

## Build

In order to build the project, follow the following steps.

- Install CMake (https://cmake.org/download/)

- Clone this repository

- Initialize submodules with ``` git submodule update --init --recursive ```

- Inside the root folder, create a build folder, and inside this folder run ``` cmake ../ ```

- Compile

- Run **julia_fractal.exe**
 
## Important

It may be necessary to change the path in the ``` getFileContent(const char* filename) ``` function calls.

## Configurations

You can change the visuals with the following keyboard keys.

- RIGH / LEFT: Increases or decreases the real part of **c**, respectively
- UP / DOWN: Increases or decreases the imaginary part of **c**, respectively
- C: Changes the **color** scale
- EQUAL (=) / MINUS (-) : Increases or decreases the **zoom**
- WASD: Moves the camera up, to the left, down, or to the right, respectively
- 1, 2, 3: Changes the evaluated **function**
