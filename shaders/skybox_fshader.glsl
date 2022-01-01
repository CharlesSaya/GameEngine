#version 330 core

uniform samplerCube skybox;

in vec2 texCoord;
in vec3 pos;

void main(void)
{
    gl_FragColor = texture( skybox, pos );
}
