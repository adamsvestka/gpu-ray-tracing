Sphere Sphere_new(vec3 center, float radius) {
    Sphere sphere;
    sphere.center = center;
    sphere.radius = radius;
    sphere.radius2 = radius * radius;
    return sphere;
}

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
