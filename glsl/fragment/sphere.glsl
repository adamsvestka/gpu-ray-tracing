Sphere Sphere_load(samplerBuffer texture, int index) {
    vec4 data = texelFetch(texture, index);
    vec4 data2 = texelFetch(texture, index + 1);
    Sphere self;
    self.center = vec3(data.z, data.w, data2.x);
    self.radius = data2.y;
    self.radius2 = data2.z;
    return self;
}

vec3 Sphere_intersect(const Sphere self, vec3 origin, vec3 direction) {
    float t0, t1;
    
    vec3 path = self.center - origin;
    float tca = dot(path, direction);
    if (tca < 0) return vec3(-1);
    
    float d2 = dot(path, path) - tca * tca;
    if (d2 > self.radius2) return vec3(-1);
    
    float thc = sqrt(self.radius2 - d2);
    t0 = tca - thc;
    t1 = tca + thc;
    if (t0 > t1) {
        float tmp = t0;
        t0 = t1;
        t1 = tmp;
    }
    
    if (t0 < 0) {
        if (t1 < 0) return vec3(-1);
        t0 = t1;
    }
    
    vec3 point = origin + direction * t0;
    return point;
}

vec3 Sphere_normal(const Sphere self, vec3 point) {
    return normalize(point - self.center);
}
