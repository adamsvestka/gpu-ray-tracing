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
    // self.Irotation = mat3(vec3(data3.w, data4.x, data4.y), vec3(data4.z, data4.w, data5.x), data5.yzw);
    self.Irotation = mat3(1);
    return self;
}

float Cuboid_intersect(const Cuboid self, vec3 origin, vec3 direction) {
    vec3 invR = 1.0 / (self.Irotation * direction);

    vec3 center = (self.vmax + self.vmin) * 0.5;

    vec3 vmax = (self.Irotation * (self.vmax - center)) + center;
    vec3 vmin = (self.Irotation * (self.vmin - center)) + center;

    vec3 ttop = invR * (vmax - origin);
    vec3 tbot = invR * (vmin - origin);

    vec3 tmin = min(ttop, tbot);
    vec3 tmax = max(ttop, tbot);

    vec2 t = max(tmin.xx, tmin.yz);

    float t0 = max(t.x, t.y);
    t = min(tmax.xx, tmax.yz);
    float t1 = min(t.x, t.y);

    if (t0 > t1) return -1;

    return t0;
}

vec3 Cuboid_normal(const Cuboid self, vec3 point) {
    vec3 size = self.vmax - self.vmin;
    vec3 _point = self.Irotation * (point - (self.vmax + self.vmin) * 0.5);

    if (abs(_point.x / size.x) > abs(_point.y / size.y) && abs(_point.x / size.x) > abs(_point.z / size.z)) return vec3(_point.x > 0 ? 1.f : -1.f, 0, 0);
    else if (abs(_point.y / size.y) > abs(_point.z / size.z)) return vec3(0, _point.y > 0 ? 1.f : -1.f, 0);
    else return vec3(0, 0, _point.z > 0 ? 1.f : -1.f);
}
