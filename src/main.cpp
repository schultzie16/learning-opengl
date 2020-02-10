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
const char *fragmentBlueShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main(){\n"
    "   FragColor = vec4(0.57f, 0.79f, 0.98f, 1.0f);\n"
    "}\0";

const char *fragmentRedShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main(){\n"
    "   FragColor = vec4(0.91f, 0.12f, 0.39f, 1.0f);\n"
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
    unsigned int fragmentBlueShader, fragmentRedShader;
    fragmentBlueShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentBlueShader, 1, &fragmentBlueShaderSource, NULL);
    glCompileShader(fragmentBlueShader);
    fragmentRedShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentRedShader, 1, &fragmentRedShaderSource, NULL);
    glCompileShader(fragmentRedShader);

    glGetShaderiv(fragmentBlueShader, GL_COMPILE_STATUS, &success);
    if (!success){
        glGetShaderInfoLog(fragmentBlueShader, sizeof(infoLog), NULL, infoLog);
        cout << "ERROR: Fragment blue shader compilation failed: " 
             << infoLog << endl;
    }

    glGetShaderiv(fragmentRedShader, GL_COMPILE_STATUS, &success);
    if (!success){
        glGetShaderInfoLog(fragmentBlueShader, sizeof(infoLog), NULL, infoLog);
        cout << "ERROR: Fragment red shader compilation failed: "
             << infoLog << endl;
    }

    // shader program links the outputs and inputs of each shader
    //   here, vertex shader --> fragment shader
    unsigned int blueShaderProgram, redShaderProgram;
    blueShaderProgram = glCreateProgram();
    redShaderProgram = glCreateProgram();
    glAttachShader(blueShaderProgram, vertexShader);
    glAttachShader(redShaderProgram, vertexShader);
    glAttachShader(blueShaderProgram, fragmentBlueShader);
    glAttachShader(redShaderProgram, fragmentRedShader);
    glLinkProgram(blueShaderProgram);
    glLinkProgram(redShaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentBlueShader);
    glDeleteShader(fragmentRedShader);

    /* ----- VERTEX DATA AND BUFFER ATTRIBUTES ----- */

    // define vertex data for blue triangle
    float blueTriVertices[] = {
        -0.75f, -0.5f, 0.0f, // left triangle - lower left
        -0.5f, 0.5f, 0.0f, // left triangle - upper
        -0.25f, -0.5f, 0.0f, // left triangle - lower right
    };

    // define vertex data for red triangle
    float redTriVertices[] = {
        0.25f, -0.5f, 0.0f,
        0.5f, 0.5f, 0.0f,
        0.75f, -0.5f, 0.0f
    };

    unsigned int indices[] = {
        0, 1, 2,
        3, 4, 5
    };

    unsigned int bVAO, bVBO, rVAO, rVBO;

    glGenVertexArrays(1, &bVAO);
    glGenVertexArrays(1, &rVAO);
    glGenBuffers(1, &bVBO);
    glGenBuffers(1, &rVBO);

    // bind the vertex array object first, bind and set vertex buffers, and
    //   configure attributes
    glBindVertexArray(bVAO);
    glBindBuffer(GL_ARRAY_BUFFER, bVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(blueTriVertices), blueTriVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 
        3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // same for the red triangle
    glBindVertexArray(rVAO);
    glBindBuffer(GL_ARRAY_BUFFER, rVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(redTriVertices), redTriVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 
        3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // glBindBuffer(GL_ARRAY_BUFFER, 0);
    // glBindVertexArray(0);

    /* ----- RENDER LOOP ----- */

    while(!glfwWindowShouldClose(myWindow)){
        // handle input
        processInput(myWindow);

        // render
        glClearColor(.11f, .44f, .64f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // draw the blue triangle
        glUseProgram(blueShaderProgram);
        glBindVertexArray(bVAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // draw the red triangle
        glUseProgram(redShaderProgram);
        glBindVertexArray(rVAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // check events and swap the buffers
        glfwSwapBuffers(myWindow);
        glfwPollEvents();
    }

    // deallocate and terminate program
    glDeleteVertexArrays(1, &bVAO);
    glDeleteVertexArrays(1, &rVAO);
    glDeleteBuffers(1, &bVBO);
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