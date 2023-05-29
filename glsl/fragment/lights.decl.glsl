struct PointLight {
    vec3 position;
    float intensity;
    vec3 color;
};

struct DirectionalLight {
    vec3 direction;
    float intensity;
    vec3 color;
};

struct GlobalLight {
    vec3 color;
    float intensity;
};

PointLight PointLight_load(samplerBuffer texture, int index) {
    vec4 data0 = texelFetch(texture, index + 0);
    vec4 data1 = texelFetch(texture, index + 1);
    vec4 data2 = texelFetch(texture, index + 2);

    PointLight light;
    light.position = vec3(data0.z, data0.w, data1.x);
    light.intensity = data1.y;
    light.color = vec3(data1.z, data1.w, data2.x);
    return light;
}

vec3 PointLight_diffuse(PointLight self, vec3 point, vec3 normal) {
    vec3 vector_to_light = self.position - point;
    float cosine_term = dot(normalize(vector_to_light), normal);
    return self.color * self.intensity * (max(cosine_term, 0.f) / (dot(vector_to_light, vector_to_light) * 4 * M_PI));
}

vec3 PointLight_specular(PointLight self, vec3 point, vec3 normal, vec3 direction, float n) {
    vec3 vector_to_light = self.position - point;
    float cosine_term = dot(normalize(vector_to_light), normal);
    return self.color * pow(self.intensity, 0.3) * pow(max(dot(normal * cosine_term * 2 - normalize(vector_to_light), -direction), 0.f), n);
}

DirectionalLight DirectionalLight_load(samplerBuffer texture, int index) {
    vec4 data0 = texelFetch(texture, index + 0);
    vec4 data1 = texelFetch(texture, index + 1);
    vec4 data2 = texelFetch(texture, index + 2);

    DirectionalLight light;
    light.direction = vec3(data0.z, data0.w, data1.x);
    light.intensity = data1.y;
    light.color = vec3(data1.z, data1.w, data2.x);
    return light;
}

vec3 DirectionalLight_diffuse(DirectionalLight self, vec3 point, vec3 normal) {
    float cosine_term = dot(normalize(self.direction), normal);
    return self.color * self.intensity * (max(cosine_term, 0.f) / (dot(self.direction, self.direction) * 4 * M_PI));
}

vec3 DirectionalLight_specular(DirectionalLight self, vec3 point, vec3 normal, vec3 direction, float n) {
    return vec3(0);
}

GlobalLight GlobalLight_load(samplerBuffer texture, int index) {
    vec4 data0 = texelFetch(texture, index + 0);
    vec4 data1 = texelFetch(texture, index + 1);
    vec4 data2 = texelFetch(texture, index + 2);

    GlobalLight light;
    light.intensity = data0.z;
    light.color = vec3(data0.w, data1.x, data1.y);
    return light;
}

vec3 GlobalLight_diffuse(GlobalLight self, vec3 point, vec3 normal) {
    return self.color * self.intensity;
}

vec3 GlobalLight_specular(GlobalLight self, vec3 point, vec3 normal, vec3 direction, float n) {
    return vec3(0);
}
