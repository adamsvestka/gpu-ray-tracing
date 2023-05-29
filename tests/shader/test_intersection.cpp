#include <doctest.h>
#include "shader_exec.hpp"

#include "common/primitives.hpp"
#include "common/shape_buffer.hpp"

TEST_CASE("test sphere intersection") {
    GLuint shapesBufferID = glGetUniformLocation(ProgramID, "shapesBuffer");
    GLuint shapesBufferSizeID = glGetUniformLocation(ProgramID, "shapesBufferSize");

    ShapeBuffer shapes(shapesBufferID, shapesBufferSizeID, 0);
    shapes.addSphere(glm::vec3(10, 0, 0), 1.0f, Material{ glm::vec3(1, 0, 0), 0.0f, 0.0f });

    float distance = SHADER_EXEC(float, Sphere_intersect(Sphere_load(shapesBuffer, 0), vec3(0, 0, 0), vec3(1, 0, 0)));

    CHECK(distance == 9.0f);
}

TEST_CASE("test sphere normal") {
    GLuint shapesBufferID = glGetUniformLocation(ProgramID, "shapesBuffer");
    GLuint shapesBufferSizeID = glGetUniformLocation(ProgramID, "shapesBufferSize");

    ShapeBuffer shapes(shapesBufferID, shapesBufferSizeID, 0);
    shapes.addSphere(glm::vec3(10, 0, 0), 1.0f, Material{ glm::vec3(1, 0, 0), 0.0f, 0.0f });

    glm::vec3 normal = SHADER_EXEC(vec3, Sphere_normal(Sphere_load(shapesBuffer, 0), vec3(9, 0, 0)));

    CHECK_VEC3_EQUAL(normal, glm::vec3(-1, 0, 0));
}

TEST_CASE("test cuboid intersection") {
    GLuint shapesBufferID = glGetUniformLocation(ProgramID, "shapesBuffer");
    GLuint shapesBufferSizeID = glGetUniformLocation(ProgramID, "shapesBufferSize");

    ShapeBuffer shapes(shapesBufferID, shapesBufferSizeID, 0);
    shapes.addCuboid(glm::vec3(10, 0, 0), glm::vec3(2, 2, 2), Material{ glm::vec3(1, 0, 0), 0.0f, 0.0f });

    float distance = SHADER_EXEC(float, Cuboid_intersect(Cuboid_load(shapesBuffer, 0), vec3(0, 0, 0), vec3(1, 0, 0)));

    CHECK(distance == 9.0f);
}

TEST_CASE("test cuboid normal") {
    GLuint shapesBufferID = glGetUniformLocation(ProgramID, "shapesBuffer");
    GLuint shapesBufferSizeID = glGetUniformLocation(ProgramID, "shapesBufferSize");

    ShapeBuffer shapes(shapesBufferID, shapesBufferSizeID, 0);
    shapes.addCuboid(glm::vec3(10, 0, 0), glm::vec3(2, 2, 2), Material{ glm::vec3(1, 0, 0), 0.0f, 0.0f });

    glm::vec3 normal = SHADER_EXEC(vec3, Cuboid_normal(Cuboid_load(shapesBuffer, 0), vec3(9, 0, 0)));

    CHECK_VEC3_EQUAL(normal, glm::vec3(-1, 0, 0));
}
