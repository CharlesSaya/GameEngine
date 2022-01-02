#version 330 core

in vec2 v_texcoord;
in vec4 v_color;

uniform sampler2D flare;
uniform float brightness;

void main()
{
    vec4 texColor = texture(flare, v_texcoord);

    gl_FragColor =  texColor* brightness;
}
