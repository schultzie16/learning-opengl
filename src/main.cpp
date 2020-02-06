#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

using std::cout;
using std::endl;

/* FUNCTION PROTOTYPES */
void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

int main(){

    cout << "A simple C++ program to test the GLFW library in VS Code!" << endl;
    cout << "---------------------------------------------------------" << endl << endl;

    /* INITIALIZE WINDOW AND SET PROPERTIES */

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* myWindow = glfwCreateWindow(800, 600, "OpenGL Demo", NULL, NULL);
    glfwMakeContextCurrent(myWindow);

    /* INITALIZE GLEW LIBRARY */
    glewExperimental = GL_TRUE;

    /* SET PROPERTIES OF VIEWPORT */

    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(myWindow, framebufferSizeCallback);

    /* WINDOW IS RUNNING */

    while(!glfwWindowShouldClose(myWindow)){
        // handle input
        processInput(myWindow);

        // render
        glClearColor(.11f, .44f, .64f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // check events and swap the buffers
        glfwSwapBuffers(myWindow);
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}

/* FUNCTION DEFINITIONS */

void framebufferSizeCallback(GLFWwindow* window, int width, int height){
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window){
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window, true);
    }
}