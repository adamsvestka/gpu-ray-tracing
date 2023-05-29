#include <doctest.h>

#include "common/primitives.hpp"
#include "common/lights.hpp"

TEST_CASE("test success") {
    CHECK(1 == 1);
}

TEST_CASE("test shape memory size") {
    auto expected = __shapes_stride * sizeof(glm::vec4);

    CHECK(sizeof(Sphere) == expected);
    CHECK(sizeof(Cuboid) == expected);
    CHECK(sizeof(Shape) == expected);
}

TEST_CASE("test light memory size") {
    auto expected = __lights_stride * sizeof(glm::vec4);

    CHECK(sizeof(PointLight) == expected);
    CHECK(sizeof(DirectionalLight) == expected);
    CHECK(sizeof(GlobalLight) == expected);
}
