#pragma once

#include <material.h>
#include <vector.h>
#include <object.h>
#include <light.h>
#include <sphere.h>

#include <algorithm>
#include <array>
#include <cassert>
#include <cstddef>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <unordered_set>
#include <vector>
#include <unordered_map>
#include <string>
#include <filesystem>
#include "triangle.h"

class Scene {
public:
    void AddObject(const Object& object) {
        objects_.push_back(object);
    }

    void AddSphereObject(const SphereObject& sphere_object) {
        sphere_objects_.push_back(sphere_object);
    }

    void AddLight(const Light& light) {
        lights_.push_back(light);
    }

    void AddMaterial(const Material& material, const std::string& name) {
        materials_.insert({name, material});
    }

    void SetMaterials(const std::unordered_map<std::string, Material>& materials) {
        materials_ = materials;
    }

    const std::vector<Object>& GetObjects() const {
        return objects_;
    }

    const std::vector<SphereObject>& GetSphereObjects() const {
        return sphere_objects_;
    }

    const std::vector<Light>& GetLights() const {
        return lights_;
    }

    const std::unordered_map<std::string, Material>& GetMaterials() const {
        return materials_;
    }

    const Material& GetMaterial(std::string name) const {
        return materials_.at(name);
    }

    bool HasMaterial(std::string name) const {
        return materials_.contains(name);
    }

private:
    std::vector<Object> objects_;
    std::vector<SphereObject> sphere_objects_;
    std::vector<Light> lights_;
    std::unordered_map<std::string, Material> materials_;
};

std::vector<std::string> Split(const std::string& string,
                               const std::unordered_set<char>& delimiters = {' ', '\t'},
                               bool enable_filtered_empty = true) {
    std::vector<std::string> result;

    if (!string.empty()) {
        auto current_string = string;
        while (true) {
            auto iterator = std::find_if(current_string.begin(), current_string.end(),
                                         [&](char const& c) { return delimiters.contains(c); });
            auto new_str = current_string.substr(0, iterator - current_string.begin());

            if (!enable_filtered_empty || !new_str.empty()) {
                result.push_back(new_str);
            }

            if (iterator == current_string.end()) {
                break;
            }

            ++iterator;
            current_string = current_string.substr(iterator - current_string.begin());
        }
    }

    return result;
}

double ToDouble(const std::string& string) {
    try {
        return std::stod(string);
    } catch (std::exception& e) {
        return 0;
    }
}

Vector ToVector(const std::vector<std::string>& data, size_t offset = 1) {
    Vector vector;
    for (size_t i = 0; i < 3; ++i) {
        assert(i + offset < data.size());
        vector[i] = ToDouble(data[i + offset]);
    }
    return vector;
}

Triangle MakeTriangle(const std::array<int, 3>& indexes, const std::vector<Vector>& data) {
    std::array<Vector, 3> vectors;

    for (size_t i = 0; i < indexes.size(); ++i) {
        if (indexes[i] == 0) {
            vectors[i] = Vector(0, 0, 0);
        } else if (indexes[i] < 0) {
            vectors[i] = data[data.size() + indexes[i]];
        } else {
            vectors[i] = data[indexes[i] - 1];
        }
    }

    return Triangle(vectors[0], vectors[1], vectors[2]);
}

std::unordered_map<std::string, Material> ReadMaterials(const std::filesystem::path& path) {
    std::ifstream file(path, std::fstream::in);
    if (!file.is_open()) {
        throw std::runtime_error{"Can't open file " + path.string()};
    }

    std::string line;
    std::unordered_map<std::string, Material> materials;
    std::string material_name;

    while (std::getline(file, line)) {
        std::vector<std::string> elements = Split(line);

        if (elements.size() < 2 || elements.front() == "#") {
            continue;
        }

        if (elements.front() == "newmtl") {
            material_name = elements[1];
            materials[material_name].name = material_name;
        } else if (elements.front() == "Ka") {
            materials[material_name].ambient_color = ToVector(elements);
        } else if (elements.front() == "Kd") {
            materials[material_name].diffuse_color = ToVector(elements);
        } else if (elements.front() == "Ks") {
            materials[material_name].specular_color = ToVector(elements);
        } else if (elements.front() == "Ke") {
            materials[material_name].intensity = ToVector(elements);
        } else if (elements.front() == "Ns") {
            materials[material_name].specular_exponent = ToDouble(elements[1]);
        } else if (elements.front() == "Ni") {
            materials[material_name].refraction_index = ToDouble(elements[1]);
        } else if (elements.front() == "al") {
            materials[material_name].albedo = ToVector(elements);
        }
    }

    file.close();
    return materials;
}

Scene ReadScene(const std::filesystem::path& path) {
    std::ifstream file(path, std::fstream::in);
    if (!file.is_open()) {
        throw std::runtime_error{"Can't open file " + path.string()};
    }

    Scene scene;
    std::string line;
    std::string material_name;
    std::array<std::vector<Vector>, 3> data;

    while (std::getline(file, line)) {
        auto elements = Split(line);

        if (elements.size() < 2 || elements.front() == "#") {
            continue;
        }

        if (elements.front() == "mtllib") {
            auto materials_file_path = path.parent_path().append(elements[1]);
            scene.SetMaterials(ReadMaterials(materials_file_path));
        } else if (elements.front() == "usemtl") {
            material_name = elements[1];
        } else if (elements.front() == "v") {
            data[0].push_back(ToVector(elements));
        } else if (elements.front() == "vt") {
            data[1].push_back(ToVector(elements));
        } else if (elements.front() == "vn") {
            data[2].push_back(ToVector(elements));
        } else if (elements.front() == "f") {
            std::array<std::vector<int>, 3> indexes;
            for (size_t i = 1; i < elements.size(); ++i) {
                auto object_data = Split(elements[i], {'/'}, false);
                for (size_t j = 0; j < 3; ++j) {
                    int value = 0;
                    if (j < object_data.size() && !object_data[j].empty()) {
                        value = std::atoi(object_data[j].c_str());
                    }
                    indexes[j].push_back(value);
                }
            }
            for (size_t i = 2; i < indexes[0].size(); ++i) {
                Object object{
                    MakeTriangle({indexes[0][0], indexes[0][i - 1], indexes[0][i]}, data[0]),
                    MakeTriangle({indexes[1][0], indexes[1][i - 1], indexes[1][i]}, data[1]),
                    MakeTriangle({indexes[2][0], indexes[2][i - 1], indexes[2][i]}, data[2]),
                };

                if (scene.HasMaterial(material_name)) {
                    object.material = &scene.GetMaterial(material_name);
                }

                scene.AddObject(object);
            }
        } else if (elements.front() == "S") {
            Sphere sphere(ToVector(elements), ToDouble(elements[4]));
            SphereObject sphere_object(sphere);

            if (scene.HasMaterial(material_name)) {
                sphere_object.material = &scene.GetMaterial(material_name);
            }

            scene.AddSphereObject(sphere_object);
        } else if (elements.front() == "P") {
            Light light(ToVector(elements), ToVector(elements, 4));
            scene.AddLight(light);
        }
    }

    file.close();
    return scene;
}
