struct Sphere {
    vec3 center;
    float radius;
    float radius2;
};

Sphere Sphere_new(vec3 center, float radius);
vec3 Sphere_intersect(const Sphere sphere, vec3 origin, vec3 direction);
