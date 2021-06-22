#include <glad/glad.h>
#include <fstream>
#include <sstream>
#include <glm/gtc/type_ptr.hpp>

#include "shader.hpp"

Shader::Shader(const std::string &vertexPath, const std::string &fragmentPath, const std::string &geometryPath) {
    auto hasGeometry = geometryPath.length() > 0;

    std::string vertexSource, fragmentSource, geometrySource;
    std::ifstream vertexFS, fragmentFS, geometryFS;

    vertexFS.exceptions(std::fstream::failbit | std::fstream::badbit);
    fragmentFS.exceptions(std::fstream::failbit | std::fstream::badbit);
    geometryFS.exceptions(std::fstream::failbit | std::fstream::badbit);

    try {
        vertexFS.open(vertexPath);
        fragmentFS.open(fragmentPath);

        if (hasGeometry) {
            geometryFS.open(geometryPath);
        }

        std::stringstream vertexSS, fragmentSS, geometrySS;
        vertexSS << vertexFS.rdbuf();
        fragmentSS << fragmentFS.rdbuf();

        if (hasGeometry) {
            geometrySS << geometryFS.rdbuf();
        }

        vertexFS.close();
        fragmentFS.close();

        if (hasGeometry) {
            geometryFS.close();
        }

        vertexSource = vertexSS.str();
        fragmentSource = fragmentSS.str();

        if (hasGeometry) {
            geometrySource = geometrySS.str();
        }
    } catch (const std::ifstream::failure &e) {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
    }

    const char *vertexSourcePtr = vertexSource.c_str();
    const char *fragmentSourcePtr = fragmentSource.c_str();

    const char *geometrySourcePtr;
    if (hasGeometry) {
        geometrySourcePtr = geometrySource.c_str();
    }

    unsigned int vertexID, fragmentID, geometryID;
    int success;
    char infoLog[512];

    vertexID = glCreateShader(GL_VERTEX_SHADER);
    fragmentID = glCreateShader(GL_FRAGMENT_SHADER);

    if (hasGeometry) {
        geometryID = glCreateShader(GL_GEOMETRY_SHADER);
    }

    glShaderSource(vertexID, 1, &vertexSourcePtr, NULL);
    glShaderSource(fragmentID, 1, &fragmentSourcePtr, NULL);

    if (hasGeometry) {
        glShaderSource(geometryID, 1, &geometrySourcePtr, NULL);
    }

    glCompileShader(vertexID);
    glGetShaderiv(vertexID, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexID, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    glCompileShader(fragmentID);
    glGetShaderiv(fragmentID, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentID, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    if (hasGeometry) {
        glCompileShader(geometryID);
        glGetShaderiv(geometryID, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(geometryID, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::GEOMETRY::COMPILATION_FAILED\n" << infoLog << std::endl;
        }
    }

    ID = glCreateProgram();
    glAttachShader(ID, vertexID);
    glAttachShader(ID, fragmentID);

    if (hasGeometry) {
        glAttachShader(ID, geometryID);
    }

    glLinkProgram(ID);
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(ID, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    glDeleteShader(vertexID);
    glDeleteShader(fragmentID);
}

Shader::~Shader() {
    glDeleteProgram(ID);
}

void Shader::use() const {
    glUseProgram(ID);
}

void Shader::setBool(const std::string &name, bool value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int) value);
}

void Shader::setInt(const std::string &name, int value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string &name, float value) const {
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setMat4(const std::string &name, glm::mat4 value) const {
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::setVec3(const std::string &name, glm::vec3 value) const {
    glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(value));
}
