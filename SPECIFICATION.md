# GPU ray tracing

**Author:** Adam Švestka

**Type:** Application

**Target Platform:** Mac

**Perceived difficulty:** 15

I want to implement a simple ray tracer, which would run on the GPU using OpenGL. I have been interested in ray tracing for a while and I think it would be a good opportunity to learn more about it and graphics programming in general.

## User interface

The resulting application would be a demo of ray tracing using OpenGL. It should be easily usable by anyone. After selecting a scene, the user should be able to move around the scene using the mouse and keyboard.

### GUI

Initially, the user selects a file with a scene description. The scene is then rendered on the screen. The user can use the mouse to rotate the camera and the keyboard to move it around. There is little extra information needed, as the viewport is the main focus. There could be extra (probably toggleable) information about the scene, such as the current fps, the number of objects in the scene, hints on usage etc. There will be a toolbar for toggling rendering steps, such as shadows, reflections, refractions, etc.

## Architecture

The ray tracing logic will be written in GLSL, using only vertex and fragment shaders for compatibility. There will be structs + functions for the following: lights, objects/shapes, and camera. And the main function for casting a ray.

C++ will control all the application logic. GUI widgets/components will be abstracted into classes using Dear ImGui, providing variable bindings, reading state and an update/render function.

GLSL code compilation will be extracted into its own function, it will support splitting the shader code into multiple files.

Finally, there will be an abstraction for loading a scene, which will be written as either json or yaml, based on which I find easier to use. The scene can contain wavefront .obj files (using a custom basic parser) and images (using stb_image). The scene will be preprocessed and all the data will be sent to the GPU.

## Development & Testing
**Build system:** Make

**Testing Framework:** Doctest

**Required libraries/frameworks:** OpenGL, GLFW, GLEW, GLM, Dear ImGui, stb, Native File Dialog, nlohmann/json, yaml-cpp

Optimally I'd like to unit test the GLSL functions, but that will be a little tricky as I have to figure out how to encode their results into an image and then decode it back.

The easiest to unit test will be loading the scene, parsing .obj files, compiling shaders, and such file operations.

I could create a scene to test performance if I have the time.

## Acceptance criteria

**Minimum Viable Product:**

- Rendering a built-in scene
  - Support for primitive objects
  - Support for multiple light sources
- Rendering the scene using ray tracing on the GPU
- Moving the camera around the scene using a mouse and keyboard
- Saving the rendered image to a file

**Extensions:**

- Loading a scene from a file
  - Support for images & simple procedural textures
  - Ability to select file to load from
  - Support for rendering meshes from .obj files
    - Reading vertices, normals, and texture coordinates
- Support for toggling rendering steps, eg. turning shadows off
- Clicking on objects to select them and show their properties
  - Such as position, size, color, etc.

**Stretch goals:**

- Editing object properties and saving the scene
- The ability to write basic shaders for objects
- Path tracing