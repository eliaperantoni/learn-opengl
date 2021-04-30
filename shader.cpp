#include <glad/glad.h>
#include <fstream>
#include <sstream>
#include <glm/gtc/type_ptr.hpp>

#include "shader.hpp"

Shader::Shader(const std::string &vertexPath, const std::string &fragmentPath) {
    std::string vertexSource, fragmentSource;
    std::ifstream vertexFS, fragmentFS;

    vertexFS.exceptions(std::fstream::failbit | std::fstream::badbit);
    fragmentFS.exceptions(std::fstream::failbit | std::fstream::badbit);

    try {
        vertexFS.open(vertexPath);
        fragmentFS.open(fragmentPath);

        std::stringstream vertexSS, fragmentSS;
        vertexSS << vertexFS.rdbuf();
        fragmentSS << fragmentFS.rdbuf();

        vertexFS.close();
        fragmentFS.close();

        vertexSource = vertexSS.str();
        fragmentSource = fragmentSS.str();
    } catch (const std::ifstream::failure &e) {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
    }

    const char *vertexSourcePtr = vertexSource.c_str();
    const char *fragmentSourcePtr = fragmentSource.c_str();

    unsigned int vertexID, fragmentID;
    int success;
    char infoLog[512];

    vertexID = glCreateShader(GL_VERTEX_SHADER);
    fragmentID = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(vertexID, 1, &vertexSourcePtr, NULL);
    glShaderSource(fragmentID, 1, &fragmentSourcePtr, NULL);

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

    ID = glCreateProgram();
    glAttachShader(ID, vertexID);
    glAttachShader(ID, fragmentID);

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
