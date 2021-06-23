#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "shader.hpp"
#include "camera.hpp"
#include "model.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(-10.0f, 0.0f, 40.0f));
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

    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    Shader unlitShader(
            "shaders/unlit/shader.vs",
            "shaders/unlit/shader.fs"
    );

    Shader asteroidShader(
            "shaders/asteroid/shader.vs",
            "shaders/asteroid/shader.fs"
    );

    Model planet ("models/planet/planet.obj");
    Model rock ("models/rock/rock.obj");

    // -----------------------------------------------------------------------------------------------------------------

#ifdef WIREFRAME
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
#endif

    // -----------------------------------------------------------------------------------------------------------------

    unsigned int amount = 100000;
    glm::mat4 *modelMatrices = new glm::mat4[amount];
    srand(glfwGetTime()); // initialize random seed
    float radius = 50.0;
    float offset = 2.5f;
    for(unsigned int i = 0; i < amount; i++)
    {
        glm::mat4 model = glm::mat4(1.0f);
        // 1. translation: displace along circle with 'radius' in range [-offset, offset]
        float angle = (float)i / (float)amount * 360.0f;
        float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
        float x = sin(angle) * radius + displacement;
        displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
        float y = displacement * 0.4f; // keep height of field smaller compared to width of x and z
        displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
        float z = cos(angle) * radius + displacement;
        model = glm::translate(model, glm::vec3(x, y, z));

        // 2. scale: scale between 0.05 and 0.25f
        float scale = (rand() % 20) / 100.0f + 0.05;
        model = glm::scale(model, glm::vec3(scale));

        // 3. rotation: add random rotation around a (semi)randomly picked rotation axis vector
        float rotAngle = (rand() % 360);
        model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));

        // 4. now add to list of matrices
        modelMatrices[i] = model;
    }

    // -----------------------------------------------------------------------------------------------------------------

    unsigned int instanceModelsVBO;
    glGenBuffers(1, &instanceModelsVBO);

    glBindBuffer(GL_ARRAY_BUFFER, instanceModelsVBO);
    glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), &modelMatrices[0], GL_STATIC_DRAW);

    for (auto i = 0; i < rock.meshes.size(); i++) {
        auto vao = rock.meshes[i].VAO;
        glBindVertexArray(vao);

        glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 4 * 4, (void *) (sizeof(float) * 4 * 0));
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 4 * 4, (void *) (sizeof(float) * 4 * 1));
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 4 * 4, (void *) (sizeof(float) * 4 * 2));
        glEnableVertexAttribArray(5);
        glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 4 * 4, (void *) (sizeof(float) * 4 * 3));
        glEnableVertexAttribArray(6);

        glVertexAttribDivisor(3, 1);
        glVertexAttribDivisor(4, 1);
        glVertexAttribDivisor(5, 1);
        glVertexAttribDivisor(6, 1);

        glBindVertexArray(0);
    }

    // -----------------------------------------------------------------------------------------------------------------

    camera.MovementSpeed *= 4;

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

        glm::mat4 model;
        glm::mat4 view;
        glm::mat4 projection;

        view = camera.GetViewMatrix();
        projection = glm::perspective(glm::radians(camera.Zoom), screenRatio, 0.1f, 200.0f);

        // -------------------------------------------------------------------------------------------------------------

        unlitShader.use();
        unlitShader.setMat4("view", view);
        unlitShader.setMat4("projection", projection);

        // -------------------------------------------------------------------------------------------------------------

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, -3.0f, 0.0f));
        model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
        unlitShader.setMat4("model", model);

        planet.Draw(unlitShader);

        // -------------------------------------------------------------------------------------------------------------

        asteroidShader.use();
        asteroidShader.setMat4("view", view);
        asteroidShader.setMat4("projection", projection);

        rock.Draw(asteroidShader, amount);

        // -------------------------------------------------------------------------------------------------------------

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
