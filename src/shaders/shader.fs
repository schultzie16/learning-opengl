#version 330 core
out vec4 FragColor;

in vec3 redVertexColor;
in vec3 vertexPosition;
in vec2 textureCoordinates;

uniform sampler2D myTexture;
uniform sampler2D myTexture2;

void main(){
    FragColor = mix(texture(myTexture, textureCoordinates), texture(myTexture2, textureCoordinates), 0.5);
}