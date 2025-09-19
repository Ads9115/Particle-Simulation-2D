#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> 
#include"Shaders.h"
#include"Circles.h"
#include<ctime>
#include"ParticleSystem.h"


const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

glm::mat4 projection;

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    float aspectRatio = (float)width / (float)height;
    projection = glm::ortho(-aspectRatio, aspectRatio, -1.0f, 1.0f, -1.0f, 1.0f);
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

int main() {
    
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        return -1;
    }

    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "OpenGL Window", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD\n";
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }

    glEnable(GL_PROGRAM_POINT_SIZE);
    srand(static_cast<unsigned int>(time(0)));

    
    ParticleSystem particleSystem(10000); 

    
    ParticleProps particleProps;
    particleProps.Position = { 0.0f, -0.9f }; 
    particleProps.Velocity = { 0.0f, 1.5f };      
    particleProps.velocityVariation = { 1.0f, 0.75f };   
    particleProps.colorStart = { 1.0f, 0.5f, 0.2f, 1.0f }; 
    particleProps.LifeTime = 1.5f;
    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    framebuffer_size_callback(window, SCR_WIDTH, SCR_HEIGHT);

    
    Shader shader("Shape.vert", "Shape.frag");

    Circle circle(glm::vec2(0.0f, 0.0f), 0.5f, 50, glm::vec4(0.2f, 0.5f, 1.0f, 1.0f));

    float lastFrameTime = 0.0f;

    double mouseX, mouseY;
    
    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        float currentTime = (float)glfwGetTime();
        float dt = currentTime - lastFrameTime;
        lastFrameTime = currentTime;

        glfwGetCursorPos(window, &mouseX, &mouseY);

        
        float ndcX = (float)(mouseX / SCR_WIDTH) * 2.0f - 1.0f;
        float ndcY = 1.0f - (float)(mouseY / SCR_HEIGHT) * 2.0f;

        particleProps.Position = { ndcX, ndcY };


        std::cout << "Delta Time (dt): " << dt << std::endl;


        
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        particleSystem.Emit(particleProps);
        particleSystem.update(dt);
        particleSystem.Draw(projection);


        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
