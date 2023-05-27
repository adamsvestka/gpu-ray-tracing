struct Cuboid {
    vec3 vmin;
    vec3 vmax;
};

Cuboid Cuboid_load(samplerBuffer texture, int index);
vec3 Cuboid_intersect(const Cuboid self, vec3 origin, vec3 direction);
vec3 Cuboid_normal(const Cuboid self, vec3 point);
