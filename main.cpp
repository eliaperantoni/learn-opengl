#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "shader.hpp"
#include "camera.hpp"
#include "model.hpp"

unsigned int loadTexture(const std::string &path);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

float deltaTime;
float lastFrame;

float screenRatio = (float) SCR_WIDTH / (float) SCR_HEIGHT;

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
    screenRatio = (float) width / (float) height;
}

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(Camera_Movement::FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(Camera_Movement::BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(Camera_Movement::LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(Camera_Movement::RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        camera.ProcessKeyboard(Camera_Movement::UP, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        camera.ProcessKeyboard(Camera_Movement::DOWN, deltaTime);
}

void mouse_callback(GLFWwindow *window, double pos_x, double pos_y) {
    if (firstMouse) {
        lastX = pos_x;
        lastY = pos_y;
        firstMouse = false;
    }

    float offset_x = pos_x - lastX;
    float offset_y = pos_y - lastY;
    offset_y *= -1;

    lastX = pos_x;
    lastY = pos_y;

    camera.ProcessMouseMovement(offset_x, offset_y);
}

void scroll_callback(GLFWwindow *window, double delta_x, double delta_y) {
    camera.ProcessMouseScroll((float) delta_y);
}

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    Shader unlitShader("shaders/unlit/shader.vs", "shaders/unlit/shader.fs");

    Model cube ("models/cube/cube.obj");
    Model plane ("models/plane/plane.obj");

    // -----------------------------------------------------------------------------------------------------------------

#ifdef WIREFRAME
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
#endif

    // -----------------------------------------------------------------------------------------------------------------

    while (!glfwWindowShouldClose(window)) {
        deltaTime = glfwGetTime() - lastFrame;
        lastFrame = glfwGetTime();

        processInput(window);

        // -------------------------------------------------------------------------------------------------------------

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);

        // -------------------------------------------------------------------------------------------------------------

        unlitShader.use();

        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), screenRatio, 0.1f, 100.0f);
        unlitShader.setMat4("projection", projection);

        unlitShader.setMat4("view", camera.GetViewMatrix());

        glm::mat4 model;

        // -------------------------------------------------------------------------------------------------------------

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, -1.01f, 0.0f));
        model = glm::scale(model, glm::vec3(4.0f));
        unlitShader.setMat4("model", model);
        plane.Draw(unlitShader);

        // -------------------------------------------------------------------------------------------------------------

        model = glm::mat4(1.0f);
        unlitShader.setMat4("model", model);
        cube.Draw(unlitShader);

        // -------------------------------------------------------------------------------------------------------------

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
