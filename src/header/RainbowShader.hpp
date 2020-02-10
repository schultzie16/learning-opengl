#ifndef RAINBOWSHADER_HPP
#define RAINBOWSHAER_HPP

#include <GL/glew.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

using std::string;
using std::stringstream;

using std::ifstream;

using std::cout;
using std::endl;

class RainbowShader {
    public:
        unsigned int ID;

        RainbowShader(const char* vertexPath, const char* fragmentPath);

        void use();

        void setBool(const string &name, bool value) const;
        void setInt(const string &name, int value) const;
        void setFloat(const string &name, float value) const;
};

#endif