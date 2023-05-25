Sphere Sphere_load(samplerBuffer texture, int index) {
    vec4 data = texelFetch(texture, index);
    vec4 data2 = texelFetch(texture, index + 1);
    Sphere sphere;
    sphere.center = data.xyz;
    sphere.radius = data.w;
    sphere.radius2 = data2.x;
    // sphere.radius = 1;
    // sphere.radius2 = 1;
    return sphere;
}

/**
 * @brief Intersect a ray with a sphere
 * @param self The sphere
 * @param origin The origin of the ray
 * @param direction The direction of the ray
 * @return The point of intersection, or vec3(0) if there is no intersection
 */
vec3 Sphere_intersect(const Sphere self, vec3 origin, vec3 direction) {
    float t0, t1;
    
    vec3 path = self.center - origin;
    float tca = dot(path, direction);
    if (tca < 0) return vec3(0);
    
    float d2 = dot(path, path) - tca * tca;
    if (d2 > self.radius2) return vec3(0);
    
    float thc = sqrt(self.radius2 - d2);
    t0 = tca - thc;
    t1 = tca + thc;
    if (t0 > t1) {
        float tmp = t0;
        t0 = t1;
        t1 = tmp;
    }
    
    if (t0 < 0) {
        if (t1 < 0) return vec3(0);
        t0 = t1;
    }
    
    vec3 point = origin + direction * t0;
    return point;
}
