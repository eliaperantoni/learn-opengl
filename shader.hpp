#pragma once
#include <iostream>

class Shader {
public:
    unsigned int ID;

    Shader(const std::string &vertexPath, const std::string &fragmentPath);
    ~Shader();

    void use() const;

    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;
};
