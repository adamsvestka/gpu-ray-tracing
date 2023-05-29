# Ray Tracing with OpenGL

The version of OpenGL available to me is 3.3, which does not support compute shaders. Therefore, I will be using a technique that does not require compute shaders.

Instead, I will be doing all the computation in a fragment shader, which will be called once for each pixel in the image. This will allow us to use the GPU to perform the ray tracing calculations in parallel. Another limitation of this older version of OpenGL is that it does not support passing arrays of structs to shaders. Therefore, we will need to serialize our shapes and store them in a texture that can be passed to the shader.

## Table of Contents

- [Ray Tracing with OpenGL](#ray-tracing-with-opengl)
  - [Table of Contents](#table-of-contents)
  - [Serialization of Shapes](#serialization-of-shapes)
    - [Shape Structs](#shape-structs)
  - [Shape Buffer in C++ and GLSL](#shape-buffer-in-c-and-glsl)
    - [Shape Buffer class in C++](#shape-buffer-class-in-c)
    - [Loading Shapes in GLSL](#loading-shapes-in-glsl)
  - [Loading and Intersecting Shapes in GLSL](#loading-and-intersecting-shapes-in-glsl)
  - [Example Implementation](#example-implementation)
  - [Conclusion](#conclusion)

## Serialization of Shapes

In order to efficiently store, manipulate, and transfer different shapes and their properties between our C++ code and our GLSL shaders, we need a method to serialize their data in a memory-efficient manner.

To accomplish this, we will store the shape data using a union of different shape structs, which will allow us to use a single buffer for all types of shapes.

Let's start by defining our shape structs. For this example, we will use spheres and cuboids.

### Shape Structs

We will define shapes using structs in C++ and their equivalent in GLSL. Here is an example of a sphere and a cuboid with a common material struct:

```cpp
struct Material {
    glm::vec3 color;
    float shininess;
    float reflectivity;
};

struct Sphere {
    glm::vec3 center;
    float radius;
    float radius_squared;
    Material material;
};

struct Cuboid {
    glm::vec3 vmin;
    vec3 vmax;
    Material material;
};
```

That's actually a simplified version of the structs we will be using. In order to store these structs in a single buffer, we need to make sure that they all have the same size and that we can distinguish between them later by giving them some kind of id. To do this, we will add padding to the structs to make sure they are all the same size, and we will add a type flag and stride flag to each struct.

```cpp
#pragma pack(push, 1)
struct Sphere {
private:
    float type = 1;
    float stride = __shapes_stride;

    glm::vec3 center;
    float radius;
    float radius_squared;
    Material material;

    float __padding[4];
};
#pragma pack(pop)
```

Since we want to store this data in a single continuous memory block, we will combine these structs into a single union:

```cpp
union Shape {
    glm::vec4 data[__shapes_stride];

    Sphere sphere;
    Cuboid cuboid;
};
```

This union helps us store both types of shapes in the same continuous region of memory that the shape buffer will use. The data array will be used to access the shape data in a linear fashion.

Now that we have defined our shapes, let's proceed with creating the shape buffer.

## Shape Buffer in C++ and GLSL

### Shape Buffer class in C++

The Shape Buffer class is responsible for creating and storing serialized shape data. We will use a custom class that inherits from our `BufferBase` class, which provides functionality for creating and updating a buffer of our serialized shapes data.

The ShapeBuffer class will provide methods to add shapes to the buffer and update their properties.

```cpp
class ShapeBuffer : public BufferBase<Shape> {
public:
    ShapeBuffer(GLuint bufferTextureID, GLuint bufferSizeID, GLuint textureID);

    SphereRef addSphere(const glm::vec3 &position, float radius, const Material &material);
    CuboidRef addCuboid(const glm::vec3 &position, glm::vec3 size, const Material &material);

    void print() const;
};
```

*addSphere()* and *addCuboid()* methods are used to add new shapes to the buffer.

*print()* method is used to print the contents of the buffer to the console.

### Loading Shapes in GLSL

Now that we have our shapes serialized and stored in a uniform buffer, we can load them in our GLSL shaders.

To do this, we will create corresponding structs for our shapes and material in GLSL:

```glsl
struct Material {
    vec3 color;
    float shininess;
    float reflectivity;
};

struct Sphere {
    vec3 center;
    float radius;
    float radius_squared;
    Material material;
};

struct Cuboid {
    vec3 vmin;
    vec3 vmax;
    Material material;
};
```

And define a uniform sampler buffer that will store our serialized shape data:

```glsl
uniform samplerBuffer shapesBuffer;
uniform int shapesBufferSize;
```

Since our shape data is linearly stored in the memory, we can access each shape using index-based pixel-fetching:

```glsl
Sphere Sphere_load(samplerBuffer texture, int index) {
    vec4 data0 = texelFetch(texture, index);
    vec4 data1 = texelFetch(texture, index + 1);
    ...
    
    Sphere self;
    self.center = vec3(data0.z, data0.w, data1.x);
    self.radius = data1.y;
    ...

    return self;
}
```

Using this method, we can load different shape types based on the type flag we store in the buffer.

## Loading and Intersecting Shapes in GLSL

With our shapes loaded in GLSL, we can now intersect rays with these shapes in our shader.

The following code demonstrates how to perform intersection testing with the loaded shapes:

```glsl
int index = 0;

while (index < shapesBufferSize) {
    vec4 data = texelFetch(shapesBuffer, index);
    switch (int(data.x)) {
        case 1: {
            Sphere sphere = Sphere_load(shapesBuffer, index);
            float newDist = Sphere_intersect(sphere, origin, direction);
            ...
            break;
        }
        case 2: {
            Cuboid cuboid = Cuboid_load(shapesBuffer, index);
            float newDist = Cuboid_intersect(cuboid, origin, direction);
            ...
            break;
        }
    }
    index += int(data.y);
}
```

## Example Implementation

With our shape buffer implemented, we can now easily define various shapes in our C++ code and use them in our shaders.

Here's an example implementation of how to use the ShapeBuffer class to define shapes in our ray tracing scene:

```cpp
GLuint shapesBufferID = glGetUniformLocation(ProgramID, "shapesBuffer");
GLuint shapesBufferSizeID = glGetUniformLocation(ProgramID, "shapesBufferSize");

ShapeBuffer shapes(shapesBufferID, shapesBufferSizeID, 0);
shapes.addSphere(glm::vec3(8.0f, 1.0f, 1.0f), 3.0f, Material{ glm::vec3(0.1f, 0.1f, 1.0f), 100.0f, 0.0f });
auto cuboid = shapes.addCuboid(glm::vec3(5.0f, 5.0f, 0.0f), glm::vec3(2.5f), Material{ glm::vec3(1.0f, 0.1f, 0.1f), 500.0f, 0.0f });
shapes.addCuboid(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(50.0f, 50.0f, 1.0f), Material{ glm::vec3(0.1f, 1.0f, 0.1f), 1000.0f, 0.5f });
```

When adding shapes to the buffer, we can store a reference to the shape in a variable. This reference can be used to update the shape properties:

```cpp
cuboid.move(glm::vec3(0.0f, delta.x, delta.y));
```

## Conclusion

The same method can be used to store other types of data in a single buffer, for example, lights.
