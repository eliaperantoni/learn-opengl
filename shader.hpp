#pragma once
#include <iostream>
#include <glm/glm.hpp>

class Shader {
public:
    unsigned int ID;

    Shader(const std::string &vertexPath, const std::string &fragmentPath, const std::string &geometryPath = "");
    ~Shader();

    void use() const;

    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;
    void setMat4(const std::string &name, glm::mat4 value) const;
    void setVec3(const std::string &name, glm::vec3 value) const;
};
