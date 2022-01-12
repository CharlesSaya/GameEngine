#version 330 core

in vec2 a_position;
in vec2 a_texcoord;

uniform vec2 screenFormat;

out vec2 v_texCoord;

void main(void)
{

    vec2 vertexPosition = ( a_position - vec2(screenFormat.x/2,screenFormat.y/2) ) / vec2(screenFormat.x/2,screenFormat.y/2 );

    v_texCoord  = a_texcoord;
    gl_Position =  vec4( vertexPosition, 0, 1 );
}
