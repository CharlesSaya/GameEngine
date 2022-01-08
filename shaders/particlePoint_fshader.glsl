#version 330 core

in vec2 v_texcoord;
in vec4 v_color;

void main()
{
    gl_FragColor = v_color;
}
