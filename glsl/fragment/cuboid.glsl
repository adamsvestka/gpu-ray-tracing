Cuboid Cuboid_load(samplerBuffer texture, int index) {
    vec4 data = texelFetch(texture, index);
    vec4 data2 = texelFetch(texture, index + 1);
    Cuboid self;
    self.vmin = vec3(data.z, data.w, data2.x);
    self.vmax = data2.yzw;
    return self;
}

vec3 Cuboid_intersect(const Cuboid self, vec3 origin, vec3 direction) {
    vec3 invR = 1.0 / direction;

    vec3 ttop = invR * (self.vmax - origin);
    vec3 tbot = invR * (self.vmin - origin);

    vec3 tmin = min(ttop, tbot);
    vec3 tmax = max(ttop, tbot);

    vec2 t = max(tmin.xx, tmin.yz);

    float t0 = max(t.x, t.y);
    t = min(tmax.xx, tmax.yz);
    float t1 = min(t.x, t.y);

    if (t0 > t1) return vec3(-1);

    return origin + direction * t0;
}

vec3 Cuboid_normal(const Cuboid self, vec3 point) {
    vec3 size = self.vmax - self.vmin;
    if (abs(point.x / size.x) > abs(point.y / size.y) && abs(point.x / size.x) > abs(point.z / size.z)) return vec3(point.x > 0 ? 1.f : -1.f, 0, 0);
    else if (abs(point.y / size.y) > abs(point.z / size.z)) return vec3(0, point.y > 0 ? 1.f : -1.f, 0);
    else return vec3(0, 0, point.z > 0 ? 1.f : -1.f);
}
