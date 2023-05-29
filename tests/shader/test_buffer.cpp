#include <doctest.h>
#include "shader_exec.hpp"

#include "common/primitives.hpp"
#include "common/shape_buffer.hpp"

TEST_CASE("test return float") {
    float value = SHADER_EXEC(float, 3.14159);
    CHECK(value == 3.14159f);
}

TEST_CASE("test return vec3") {
    glm::vec3 center = SHADER_EXEC(vec3, vec3(1, 2, 3));
    CHECK_VEC3_EQUAL(center, glm::vec3(1, 2, 3));
}

TEST_CASE("test sphere_load") {
    GLuint shapesBufferID = glGetUniformLocation(ProgramID, "shapesBuffer");
    GLuint shapesBufferSizeID = glGetUniformLocation(ProgramID, "shapesBufferSize");

    ShapeBuffer shapes(shapesBufferID, shapesBufferSizeID, 0);
    shapes.addSphere(glm::vec3(1, 2, 3), 4.0f, Material{ glm::vec3(1, 0, 0), 0.0f, 0.0f });

    Sphere sphere(
        SHADER_EXEC(vec3, Sphere_load(shapesBuffer, 0).center),
        SHADER_EXEC(float, Sphere_load(shapesBuffer, 0).radius),
        Material{
            SHADER_EXEC(vec3, Sphere_load(shapesBuffer, 0).material.color),
            SHADER_EXEC(float, Sphere_load(shapesBuffer, 0).material.shininess),
            SHADER_EXEC(float, Sphere_load(shapesBuffer, 0).material.reflectivity)
        }
    );

    CHECK_VEC3_EQUAL(sphere.getPosition(), glm::vec3(1, 2, 3));
    CHECK(sphere.radius == 4.0f);
    CHECK_VEC3_EQUAL(sphere.material.color, glm::vec3(1, 0, 0));
    CHECK(sphere.material.shininess == 0.0f);
    CHECK(sphere.material.reflectivity == 0.0f);
}

TEST_CASE("test sphere_load with multiple spheres") {
    GLuint shapesBufferID = glGetUniformLocation(ProgramID, "shapesBuffer");
    GLuint shapesBufferSizeID = glGetUniformLocation(ProgramID, "shapesBufferSize");

    ShapeBuffer shapes(shapesBufferID, shapesBufferSizeID, 0);
    shapes.addSphere(glm::vec3(1, 2, 3), 4.0f, Material{ glm::vec3(1, 0, 0), 0.0f, 0.0f });
    shapes.addSphere(glm::vec3(4, 5, 6), 7.0f, Material{ glm::vec3(0, 1, 0), 0.0f, 0.0f });

    Sphere sphere1(
        SHADER_EXEC(vec3, Sphere_load(shapesBuffer, 0).center),
        SHADER_EXEC(float, Sphere_load(shapesBuffer, 0).radius),
        Material{
            SHADER_EXEC(vec3, Sphere_load(shapesBuffer, 0).material.color),
            SHADER_EXEC(float, Sphere_load(shapesBuffer, 0).material.shininess),
            SHADER_EXEC(float, Sphere_load(shapesBuffer, 0).material.reflectivity)
        }
    );

    Sphere sphere2(
        SHADER_EXEC(vec3, Sphere_load(shapesBuffer, 4).center),
        SHADER_EXEC(float, Sphere_load(shapesBuffer, 4).radius),
        Material{
            SHADER_EXEC(vec3, Sphere_load(shapesBuffer, 4).material.color),
            SHADER_EXEC(float, Sphere_load(shapesBuffer, 4).material.shininess),
            SHADER_EXEC(float, Sphere_load(shapesBuffer, 4).material.reflectivity)
        }
    );

    CHECK_VEC3_EQUAL(sphere1.getPosition(), glm::vec3(1, 2, 3));
    CHECK(sphere1.radius == 4.0f);
    CHECK_VEC3_EQUAL(sphere1.material.color, glm::vec3(1, 0, 0));
    CHECK(sphere1.material.shininess == 0.0f);
    CHECK(sphere1.material.reflectivity == 0.0f);

    CHECK_VEC3_EQUAL(sphere2.getPosition(), glm::vec3(4, 5, 6));
    CHECK(sphere2.radius == 7.0f);
    CHECK_VEC3_EQUAL(sphere2.material.color, glm::vec3(0, 1, 0));
    CHECK(sphere2.material.shininess == 0.0f);
    CHECK(sphere2.material.reflectivity == 0.0f);
}

TEST_CASE("test cuboid_load") {
    GLuint shapesBufferID = glGetUniformLocation(ProgramID, "shapesBuffer");
    GLuint shapesBufferSizeID = glGetUniformLocation(ProgramID, "shapesBufferSize");

    ShapeBuffer shapes(shapesBufferID, shapesBufferSizeID, 0);
    shapes.addCuboid(glm::vec3(1, 2, 3), glm::vec3(4, 5, 6), Material{ glm::vec3(1, 0, 0), 0.0f, 0.0f });

    Cuboid cuboid(
        SHADER_EXEC(vec3, (Cuboid_load(shapesBuffer, 0).vmin + Cuboid_load(shapesBuffer, 0).vmax) / 2),
        SHADER_EXEC(vec3, Cuboid_load(shapesBuffer, 0).vmax - Cuboid_load(shapesBuffer, 0).vmin),
        Material{
            SHADER_EXEC(vec3, Cuboid_load(shapesBuffer, 0).material.color),
            SHADER_EXEC(float, Cuboid_load(shapesBuffer, 0).material.shininess),
            SHADER_EXEC(float, Cuboid_load(shapesBuffer, 0).material.reflectivity)
        }
    );

    CHECK_VEC3_EQUAL(cuboid.getPosition(), glm::vec3(1, 2, 3));
    CHECK_VEC3_EQUAL(cuboid.getSize(), glm::vec3(4, 5, 6));
    CHECK_VEC3_EQUAL(cuboid.material.color, glm::vec3(1, 0, 0));
    CHECK(cuboid.material.shininess == 0.0f);
    CHECK(cuboid.material.reflectivity == 0.0f);
}

TEST_CASE("test cuboid_load with multiple cuboids") {
    GLuint shapesBufferID = glGetUniformLocation(ProgramID, "shapesBuffer");
    GLuint shapesBufferSizeID = glGetUniformLocation(ProgramID, "shapesBufferSize");

    ShapeBuffer shapes(shapesBufferID, shapesBufferSizeID, 0);
    shapes.addCuboid(glm::vec3(1, 2, 3), glm::vec3(4, 5, 6), Material{ glm::vec3(1, 0, 0), 0.0f, 0.0f });
    shapes.addCuboid(glm::vec3(4, 5, 6), glm::vec3(7, 8, 9), Material{ glm::vec3(0, 1, 0), 0.0f, 0.0f });

    Cuboid cuboid1(
        SHADER_EXEC(vec3, (Cuboid_load(shapesBuffer, 0).vmin + Cuboid_load(shapesBuffer, 0).vmax) / 2),
        SHADER_EXEC(vec3, Cuboid_load(shapesBuffer, 0).vmax - Cuboid_load(shapesBuffer, 0).vmin),
        Material{
            SHADER_EXEC(vec3, Cuboid_load(shapesBuffer, 0).material.color),
            SHADER_EXEC(float, Cuboid_load(shapesBuffer, 0).material.shininess),
            SHADER_EXEC(float, Cuboid_load(shapesBuffer, 0).material.reflectivity)
        }
    );

    Cuboid cuboid2(
        SHADER_EXEC(vec3, (Cuboid_load(shapesBuffer, 4).vmin + Cuboid_load(shapesBuffer, 4).vmax) / 2),
        SHADER_EXEC(vec3, Cuboid_load(shapesBuffer, 4).vmax - Cuboid_load(shapesBuffer, 4).vmin),
        Material{
            SHADER_EXEC(vec3, Cuboid_load(shapesBuffer, 4).material.color),
            SHADER_EXEC(float, Cuboid_load(shapesBuffer, 4).material.shininess),
            SHADER_EXEC(float, Cuboid_load(shapesBuffer, 4).material.reflectivity)
        }
    );

    CHECK_VEC3_EQUAL(cuboid1.getPosition(), glm::vec3(1, 2, 3));
    CHECK_VEC3_EQUAL(cuboid1.getSize(), glm::vec3(4, 5, 6));
    CHECK_VEC3_EQUAL(cuboid1.material.color, glm::vec3(1, 0, 0));
    CHECK(cuboid1.material.shininess == 0.0f);
    CHECK(cuboid1.material.reflectivity == 0.0f);

    CHECK_VEC3_EQUAL(cuboid2.getPosition(), glm::vec3(4, 5, 6));
    CHECK_VEC3_EQUAL(cuboid2.getSize(), glm::vec3(7, 8, 9));
    CHECK_VEC3_EQUAL(cuboid2.material.color, glm::vec3(0, 1, 0));
    CHECK(cuboid2.material.shininess == 0.0f);
    CHECK(cuboid2.material.reflectivity == 0.0f);
}

TEST_CASE("test sphere_load and cuboid_load with multiple shapes") {
    GLuint shapesBufferID = glGetUniformLocation(ProgramID, "shapesBuffer");
    GLuint shapesBufferSizeID = glGetUniformLocation(ProgramID, "shapesBufferSize");

    ShapeBuffer shapes(shapesBufferID, shapesBufferSizeID, 0);
    shapes.addSphere(glm::vec3(1, 2, 3), 4.0f, Material{ glm::vec3(1, 0, 0), 0.0f, 0.0f });
    shapes.addCuboid(glm::vec3(4, 5, 6), glm::vec3(7, 8, 9), Material{ glm::vec3(0, 1, 0), 0.0f, 0.0f });

    Sphere sphere(
        SHADER_EXEC(vec3, Sphere_load(shapesBuffer, 0).center),
        SHADER_EXEC(float, Sphere_load(shapesBuffer, 0).radius),
        Material{
            SHADER_EXEC(vec3, Sphere_load(shapesBuffer, 0).material.color),
            SHADER_EXEC(float, Sphere_load(shapesBuffer, 0).material.shininess),
            SHADER_EXEC(float, Sphere_load(shapesBuffer, 0).material.reflectivity)
        }
    );

    Cuboid cuboid(
        SHADER_EXEC(vec3, (Cuboid_load(shapesBuffer, 4).vmin + Cuboid_load(shapesBuffer, 4).vmax) / 2),
        SHADER_EXEC(vec3, Cuboid_load(shapesBuffer, 4).vmax - Cuboid_load(shapesBuffer, 4).vmin),
        Material{
            SHADER_EXEC(vec3, Cuboid_load(shapesBuffer, 4).material.color),
            SHADER_EXEC(float, Cuboid_load(shapesBuffer, 4).material.shininess),
            SHADER_EXEC(float, Cuboid_load(shapesBuffer, 4).material.reflectivity)
        }
    );

    CHECK_VEC3_EQUAL(sphere.getPosition(), glm::vec3(1, 2, 3));
    CHECK(sphere.radius == 4.0f);
    CHECK_VEC3_EQUAL(sphere.material.color, glm::vec3(1, 0, 0));
    CHECK(sphere.material.shininess == 0.0f);
    CHECK(sphere.material.reflectivity == 0.0f);

    CHECK_VEC3_EQUAL(cuboid.getPosition(), glm::vec3(4, 5, 6));
    CHECK_VEC3_EQUAL(cuboid.getSize(), glm::vec3(7, 8, 9));
    CHECK_VEC3_EQUAL(cuboid.material.color, glm::vec3(0, 1, 0));
    CHECK(cuboid.material.shininess == 0.0f);
    CHECK(cuboid.material.reflectivity == 0.0f);
}
