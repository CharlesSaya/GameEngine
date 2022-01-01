#version 330 core

in vec2 v_texcoord;
in vec4 v_color;

uniform sampler2D sprite;

void main()
{
    gl_FragColor = texture(sprite, v_texcoord) * v_color;
}
