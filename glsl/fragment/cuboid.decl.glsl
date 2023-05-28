struct Cuboid {
    vec3 vmin;
    vec3 vmax;
    Material material;
    mat3 Irotation;
};

Cuboid Cuboid_load(samplerBuffer texture, int index);
float Cuboid_intersect(const Cuboid self, vec3 origin, vec3 direction);
vec3 Cuboid_normal(const Cuboid self, vec3 point);
