#include <GLFW/glfw3.h>
#include <iostream>
using namespace std;

void processInput(GLFWwindow *w);

int main(){
    cout << "A simple C++ program to test the GLFW library in VS Code!" << endl;
    cout << "---------------------------------------------------------" << endl << endl;

    /* initialize and determine window properties */

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    /* create game window */
    // glfwCreateWindow(): width, height, window name, monitor, share
    GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL Test", NULL, NULL);
    if (window == NULL){
        cout << "Failed to create GLFW Window!" << endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    while(!glfwWindowShouldClose(window)){
        processInput(window);

        glClearColor(0.51f, 0.85f, 0.98f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // glfwSwapBuffers() swaps the color buffer used to render this iteration
        glfwSwapBuffers(window);
        /// glfwPollEvents() checks for events and updates the window state
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow *w){
    if (glfwGetKey(w, GLFW_KEY_SPACE) == GLFW_PRESS){
        glfwSetWindowShouldClose(w, true);
    }
}