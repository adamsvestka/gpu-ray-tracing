# GPU Ray Tracing

This project demonstrates a real-time ray tracing implementation using OpenGL (OpenGL version 3.3) and modern hardware. Ray tracing is a rendering technique that simulates the physical behavior of light to generate realistic and visually accurate 3D images. The implementation uses a fragment shader and a technique for storing serialized shape data in a texture, allowing efficient data manipulation between C++ code and GLSL shaders.

**Author:** Adam Švestka

## Features

- Real-time ray tracing of scenes with different shapes, such as spheres and cuboids, with accurate shading, reflections, and shadows.
- Controls for moving the camera, changing the render mode, and adjusting light properties.
- Serialization of shape data using a union of different shape structs.
- Loading and intersecting of serialized shapes in GLSL shaders.
- Example implementation of a shape buffer in C++ and GLSL.
- Makefile system and helper functions for managing compilation and execution.

## Installation

```sh
git clone git@github.com:adamsvestka/gpu-ray-tracing.git
cd gpu-ray-tracing
make run
```

Make will *hopefully* take care of everything.

*Tested on macOS 13.*

## Controls

- `W`, `A`, `S`, `D` - move
- `space`, `left shift` - move up/down
- `mouse drag left button` - look around

There is some information in the upper left corner:
- `FPS` - frames per second
- `Camera position` - camera position in world space
- `Camera rotation` - horizontal and vertical rotation of the camera in degrees
- `Render mode` - currently selected render mode
  - Ray Tracing - ray tracing with shadows and reflections
  - Normals - show normals of objects in the scene, useful for debugging
  - Depth - show distance of objects from the camera
  - Color - show unshaded colors of objects
- `Light position` - position of a sample light source in the scene, can be moved around
- `Light intensity` - intensity of the sample light source

## Testing

The project uses doctest for unit testing. To run the tests, use the following command:

```sh
make test
```

## Implementation Details

The GPU ray tracing implementation is based on the following concepts and techniques:

- Serialization of shape data using a union of different shape structs, such as spheres and cuboids.
- Storage and manipulation of serialized shape data in a texture, allowing efficient transfer between C++ code and GLSL shaders.
- Use of a fragment shader to perform ray tracing calculations, exploiting the parallelism provided by modern GPUs.
- Example code for creating a shape buffer in C++ and GLSL, allowing multiple shapes to be defined and updated in the scene.
- Intersection tests and shading algorithms implemented in GLSL shaders.

For a detailed explanation of the implementation process and examples, refer to the provided markdown files in the project repository.
