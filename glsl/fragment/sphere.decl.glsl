struct Sphere {
    vec3 center;
    float radius;
    float radius2;
};

Sphere Sphere_load(samplerBuffer texture, int index);
vec3 Sphere_intersect(const Sphere self, vec3 origin, vec3 direction);
vec3 Sphere_normal(const Sphere self, vec3 point);
