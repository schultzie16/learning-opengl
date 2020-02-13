/* Chris Schultz
 * 6 Feb 2020
 * Learn OpenGL Project
 * main.cpp
 */

#define STB_IMAGE_IMPLEMENTATION

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "../header/stb_image.h"

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
          /* positions */      /* colors */     /* texture coord */
         0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,      1.0f, 0.0f,   /* lower right corner */
        -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,      0.0f, 0.0f,   /* lower left corner */
         0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f,      0.5f, 1.0f    /* upper point */
    };

    unsigned int rVAO, rVBO;

    glGenVertexArrays(1, &rVAO);
    glGenBuffers(1, &rVBO);

    // bind the vertex array object first, bind and set vertex buffers, and
    //   configure attributes
    glBindVertexArray(rVAO);
    glBindBuffer(GL_ARRAY_BUFFER, rVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(redTriVertices), redTriVertices, GL_STATIC_DRAW);
    
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 
        8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // color atrribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
        8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    // texture attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 
        8 * sizeof(float), (void*) (6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    /* ----- TEXTURE SET UP ----- */

    // load and create the texture
    unsigned int texture, texture2;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // set the texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // load the image, create the texture, generate mipmaps
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load("textures/wall.jpg", &width, &height, &nrChannels, 0);
    if (data){
        // args: texture target, mipmap level, texture storage format, width, height, always 0,
        //   format of source image, datatype of source image, image data
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        cout << "Failed to load texture!" << endl;
    }
    stbi_image_free(data);

    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    data = stbi_load("textures/peter.jpg", &width, &height, &nrChannels, 0);
    if (data){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        cout << "Failed to load texture!" << endl;
    }
    stbi_image_free(data);

    /* ----- RENDER LOOP ----- */

    float offset = 0.5f;
    rainbowShader.use();
    glUniform1i(glGetUniformLocation(rainbowShader.ID, "myTexture"), 0);
    rainbowShader.setInt("myTexture2", 1);


    while(!glfwWindowShouldClose(myWindow)){
        // handle input
        processInput(myWindow);

        // render
        glClearColor(.11f, .44f, .64f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        glBindVertexArray(rVAO);
        rainbowShader.use();
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