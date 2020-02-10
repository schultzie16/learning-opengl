#include "../header/RainbowShader.hpp"

RainbowShader::RainbowShader(const char* vertexPath, const char* fragmentPath){

    /* ----- RETRIEVE THE VERTEX AND FRAGMENT CODE FROM FILEPATH -----*/

    string vertexCode;
    string fragmentCode;
    ifstream vShaderFile;
    ifstream fShaderFile;

    // ensure that the files can throw exceptions
    vShaderFile.exceptions (ifstream::failbit | ifstream::badbit);
    fShaderFile.exceptions (ifstream::failbit | ifstream::badbit);
    try {
        // open the files
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        stringstream vShaderStream, fShaderStream;

        // read the file's buffer contents into streams
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();

        // close the file handlers
        vShaderFile.close();
        fShaderFile.close();
        
        // convert the stream into a string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch (ifstream::failure e){
        cout << "ERROR: SHADER FILE NOT SUCCESSFULLY READ" << endl;
    }

    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    /* ----- SHADER COMPILATION ----- */

    unsigned int vertex, fragment;
    int success;
    char infoLog[512];

    // build and compile vertex shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);

    // test for compilation errors
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success){
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        cout << "ERROR: VERTEX SHADER COMPILATION FAILED" << infoLog << endl;
    }

    // build and compile fragment shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);

    // test for compilation errors
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success){
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        cout << "ERROR: FRAGMENT SHADER COMPILATION FAILED" << infoLog << endl;
    }

    // create the shader program
    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);

    // test for linking errors
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if (!success){
        glGetProgramInfoLog(ID, 512, NULL, infoLog);
        cout << "ERROR: SHADER PROGRAM LINKING FAILED" << infoLog << endl;
    }

    // delete the shaders
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void RainbowShader::use(){
    glUseProgram(ID);
}

void RainbowShader::setBool(const string &name, bool value) const{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int) value);
}

void RainbowShader::setInt(const string &name, int value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void RainbowShader::setFloat(const string &name, float value) const{
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}