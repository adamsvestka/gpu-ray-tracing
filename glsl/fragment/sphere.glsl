Sphere Sphere_load(samplerBuffer texture, int index) {
    vec4 data0 = texelFetch(texture, index + 0);
    vec4 data1 = texelFetch(texture, index + 1);
    vec4 data2 = texelFetch(texture, index + 2);
    vec4 data3 = texelFetch(texture, index + 3);
    vec4 data4 = texelFetch(texture, index + 4);
    vec4 data5 = texelFetch(texture, index + 5);

    Sphere self;
    self.center = vec3(data0.z, data0.w, data1.x);
    self.radius = data1.y;
    self.radius_squared = data1.z;
    self.material.color = vec3(data1.w, data2.x, data2.y);
    self.material.specular = data2.z;
    self.material.reflectivity = data2.w;
    return self;
}

float Sphere_intersect(const Sphere self, vec3 origin, vec3 direction) {
    float t0, t1;

    vec3 path = self.center - origin;
    float tca = dot(path, direction);
    if (tca < 0) return -1;

    float d2 = dot(path, path) - tca * tca;
    if (d2 > self.radius_squared) return -1;

    float thc = sqrt(self.radius_squared - d2);
    t0 = tca - thc;
    t1 = tca + thc;
    if (t0 > t1) {
        float tmp = t0;
        t0 = t1;
        t1 = tmp;
    }

    if (t0 < 0) {
        if (t1 < 0) return -1;
        t0 = t1;
    }

    return t0;
}

vec3 Sphere_normal(const Sphere self, vec3 point) {
    return normalize(point - self.center);
}
