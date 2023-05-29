Cuboid Cuboid_load(samplerBuffer texture, int index) {
    vec4 data0 = texelFetch(texture, index + 0);
    vec4 data1 = texelFetch(texture, index + 1);
    vec4 data2 = texelFetch(texture, index + 2);
    vec4 data3 = texelFetch(texture, index + 3);
    vec4 data4 = texelFetch(texture, index + 4);
    vec4 data5 = texelFetch(texture, index + 5);

    Cuboid self;
    self.vmin = vec3(data0.z, data0.w, data1.x);
    self.vmax = vec3(data1.y, data1.z, data1.w);
    self.material.color = data2.xyz;
    self.material.specular = data2.w;
    self.material.reflectivity = data3.x;
    self.material.ior = data3.y;
    self.material.transparent = data3.z;
    return self;
}

float Cuboid_intersect(const Cuboid self, vec3 origin, vec3 direction) {
    vec3 dirfrac = vec3(1.0f / direction.x, 1.0f / direction.y, 1.0f / direction.z);

    float t1 = (self.vmin.x - origin.x) * dirfrac.x;
    float t2 = (self.vmax.x - origin.x) * dirfrac.x;
    float t3 = (self.vmin.y - origin.y) * dirfrac.y;
    float t4 = (self.vmax.y - origin.y) * dirfrac.y;
    float t5 = (self.vmin.z - origin.z) * dirfrac.z;
    float t6 = (self.vmax.z - origin.z) * dirfrac.z;

    float tmin = max(max(min(t1, t2), min(t3, t4)), min(t5, t6));
    float tmax = min(min(max(t1, t2), max(t3, t4)), max(t5, t6));

    if (tmax < 0) return -1;
    if (tmin > tmax) return -1;

    return tmin;
}

vec3 Cuboid_normal(const Cuboid self, vec3 point) {
    vec3 size = self.vmax - self.vmin;
    vec3 _point = point - (self.vmax + self.vmin) * 0.5;

    if (abs(_point.x / size.x) > abs(_point.y / size.y) && abs(_point.x / size.x) > abs(_point.z / size.z)) return vec3(_point.x > 0 ? 1.f : -1.f, 0, 0);
    else if (abs(_point.y / size.y) > abs(_point.z / size.z)) return vec3(0, _point.y > 0 ? 1.f : -1.f, 0);
    else return vec3(0, 0, _point.z > 0 ? 1.f : -1.f);
}
