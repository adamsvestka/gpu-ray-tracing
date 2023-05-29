# GPU ray tracing

**Author:** Adam Švestka

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
