/* Chris Schultz
 * 6 Feb 2020
 * Learn OpenGL Project
 * main.cpp
 */



#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "../header/RainbowShader.hpp"

#include <iostream>
#include <cmath>

using std::cout;
using std::endl;

// function prototypes
void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

int main(){

    cout << "A simple C++ program to test the GLFW library in VS Code!" << endl;
    cout << "---------------------------------------------------------" << endl << endl;

    /* ----- INITIALIZATION ----- */

    // initalize window and set properties
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* myWindow = glfwCreateWindow(
        800, 600, "OpenGL Demo", NULL, NULL);
    glfwMakeContextCurrent(myWindow);
    glfwSetFramebufferSizeCallback(myWindow, framebufferSizeCallback);

    // initialize glew library
    glewExperimental = GL_TRUE;
    glewInit();

    /* ----- SHADER COMPILATION ----- */

    RainbowShader rainbowShader("src/shaders/shader.vs", "src/shaders/shader.fs");

    /* ----- VERTEX DATA AND BUFFER ATTRIBUTES ----- */

    // define vertex data for red triangle
    float redTriVertices[] = {
          /* positions */      /* colors */
         0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   /* lower right corner */
        -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   /* lower left corner */
         0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    /* upper point */
    };

    unsigned int rVAO, rVBO;

    glGenVertexArrays(1, &rVAO);
    glGenBuffers(1, &rVBO);

    // bind the vertex array object first, bind and set vertex buffers, and
    //   configure attributes
    glBindVertexArray(rVAO);
    glBindBuffer(GL_ARRAY_BUFFER, rVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(redTriVertices), redTriVertices, GL_STATIC_DRAW);
    // set the position attributes - need to shift 6 floats to the right to get new value
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 
        6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // same with the color attributes
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 
        6 * sizeof(float), (void*) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    /* ----- RENDER LOOP ----- */

    float offset = 0.5f;

    while(!glfwWindowShouldClose(myWindow)){
        // handle input
        processInput(myWindow);

        // render
        glClearColor(.11f, .44f, .64f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(rVAO);
        rainbowShader.use();
        rainbowShader.setFloat("xOffset", offset);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // check events and swap the buffers
        glfwSwapBuffers(myWindow);
        glfwPollEvents();
    }

    // deallocate and terminate program
    glDeleteVertexArrays(1, &rVAO);
    glDeleteBuffers(1, &rVBO);

    glfwTerminate();

    return 0;
}


/* ----- FUNCTION DEFINITIONS ----- */

void framebufferSizeCallback(GLFWwindow* window, int width, int height){
    glViewport(0, 0, width, height);
}

// press ESC to exit program
void processInput(GLFWwindow* window){
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window, true);
    }
}