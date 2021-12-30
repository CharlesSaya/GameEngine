#version 330 core

uniform samplerCube skybox;
uniform sampler2D shadowTexture;

in vec2 texCoord;
in vec3 pos;

void main(void)
{
    gl_FragColor = texture( skybox, pos );
}
