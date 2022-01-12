#version 330 core

uniform sampler2D fontAtlas;

in vec2 v_texCoord;

void main(void)
{
    gl_FragColor = texture(fontAtlas, v_texCoord);
}
