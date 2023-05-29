// #include "scene_loader.hpp"

// #include <GL/glew.h>

// glm::vec3 parse_vec3(const YAML::Node &node) {
//     return glm::vec3(node[0].as<float>(), node[1].as<float>(), node[2].as<float>());
// }

// glm::vec3 parse_color(const YAML::Node &node) {
//     std::string hex = node.as<std::string>();
//     return glm::vec3(
//         std::stoi(hex.substr(0, 2), nullptr, 16) / 255.0f,
//         std::stoi(hex.substr(2, 2), nullptr, 16) / 255.0f,
//         std::stoi(hex.substr(4, 2), nullptr, 16) / 255.0f
//     );
// }

// Material parse_material(const YAML::Node &node) {
//     return Material{ parse_color(node["color"]), node["shininess"].as<float>(0.f), node["reflectivity"].as<float>(0.f) };
// }

// void parse_shape(const YAML::Node &node, ShapeBuffer &shape_buffer) {
//     std::string type = node["type"].as<std::string>();
//     if (type == "sphere") {
//         shape_buffer.addSphere(parse_vec3(node["position"]), node["radius"].as<float>(), parse_material(node["material"]));
//     } else if (type == "cuboid") {
//         shape_buffer.addCuboid(parse_vec3(node["position"]), parse_vec3(node["size"]), parse_material(node["material"]));
//     } else if (type == "plane") {
//         // shape_buffer.addPlane(parse_vec3(node["position"]), parse_vec3(node["normal"]), parse_material(node["material"]));
//     } else {
//         throw std::runtime_error("Unknown shape type: " + type);
//     }
// }

// void parse_light(const YAML::Node &node, LightBuffer &light_buffer) {
//     std::string type = node["type"].as<std::string>();
//     if (type == "point") {
//         light_buffer.addPointLight(parse_vec3(node["position"]), parse_color(node["color"]), node["intensity"].as<float>());
//     } else if (type == "directional") {
//         light_buffer.addDirectionalLight(parse_vec3(node["direction"]), parse_color(node["color"]), node["intensity"].as<float>());
//     } else if (type == "global") {
//         light_buffer.addGlobalLight(parse_color(node["color"]), node["intensity"].as<float>());
//     } else {
//         throw std::runtime_error("Unknown light type: " + type);
//     }
// }

// void SceneLoader::load_yaml(const std::string &filename, Camera &camera, ShapeBuffer &shape_buffer, LightBuffer &light_buffer) {
//     YAML::Node node = YAML::LoadFile(filename);

//     camera.position = parse_vec3(node["camera"]["position"]);
//     camera.rotation = parse_vec3(node["camera"]["rotation"]);

//     for (const auto &shape_node : node["shapes"]) {
//         parse_shape(shape_node, shape_buffer);
//     }

//     for (const auto &light_node : node["lights"]) {
//         parse_light(light_node, light_buffer);
//     }
// }
