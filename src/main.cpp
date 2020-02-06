/* Chris Schultz
 * 6 Feb 2020
 * Learn OpenGL Project
 * main.cpp
 */

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

using std::cout;
using std::endl;

/* ----- SHADER DEFINITIONS ----- */

// vertex shader handles the individual vertices
const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

// fragment shader calculates the color output
const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main(){\n"
    "   FragColor = vec4(0.58f, 0.09f, 0.47f, 1.0f);\n"
    "}\0";

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
    
    // build and compile vertex shader
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // test for successful compilation
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success){
        glGetShaderInfoLog(vertexShader, sizeof(infoLog), NULL, infoLog);
        cout << "ERROR: Vertex shader compilation failed: " << infoLog << endl;
    }

    // build, compile, and test the fragment shader
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success){
        glGetShaderInfoLog(vertexShader, sizeof(infoLog), NULL, infoLog);
        cout << "ERROR: Fragment shader compilation failed: " 
             << infoLog << endl;
    }

    // shader program links the outputs and inputs of each shader
    //   here, vertex shader --> fragment shader
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    /* ----- VERTEX DATA AND BUFFER ATTRIBUTES ----- */

    // define vertex data
    float vertices[] = {
        -0.5f, 0.5f, 0.0f, // top-left
        0.5f, 0.5f, 0.0f, // top-right
        -0.5f, -0.5f, 0.0f, // bottom-left
        0.5f, -0.5f, 0.0f, // bottom-right
    };

    unsigned int indices[] = {
        0, 2, 3,
        0, 1, 3
    };

    unsigned int VAO, VBO, EBO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // bind the vertex array object first, bind and set vertex buffers, and
    //   configure attributes
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), 
        indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 
        3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    /* ----- RENDER LOOP ----- */

    while(!glfwWindowShouldClose(myWindow)){
        // handle input
        processInput(myWindow);

        // render
        glClearColor(.11f, .44f, .64f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // draw a triangle
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // check events and swap the buffers
        glfwSwapBuffers(myWindow);
        glfwPollEvents();
    }

    // deallocate and terminate program
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

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