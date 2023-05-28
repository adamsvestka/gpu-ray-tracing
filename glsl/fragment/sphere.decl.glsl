struct Sphere {
    vec3 center;
    float radius;
    float radius_squared;
    Material material;
    // mat4 transform;
};

Sphere Sphere_load(samplerBuffer texture, int index);
float Sphere_intersect(const Sphere self, vec3 origin, vec3 direction);
vec3 Sphere_normal(const Sphere self, vec3 point);
