float width = resolution.x;
float height = resolution.y;
float scale = 1;

vec3 getCameraRay(vec2 pos) {
    float u = (2 * (pos.x + 0.5) / width - 1) * width / height * scale;
    float v = (1 - 2 * (pos.y + 0.5) / height) * scale;
    
    return /*rotation * */normalize(vec3(1, u, v));
}

vec3 calculateLighting(vec3 point, vec3 normal, vec3 direction, int intensity, int n) {
    vec3 vector_to_light = light - point;
    
    vec3 ambient = vec3(0.1);
    float cosine_term = dot(normalize(vector_to_light), normal);
    
    return  vec3(1) * intensity * max(cosine_term, 0.f) / (dot(vector_to_light, vector_to_light) * 4 * M_PI) 
            + vec3(1) * pow(intensity, 0.3) * pow(max(dot(-(normal * (cosine_term * 2) - normalize(vector_to_light)), direction), 0.f), n)
            + ambient;
}
