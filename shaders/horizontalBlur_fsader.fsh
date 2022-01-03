#version 330

uniform sampler2D target;

in vec3 v_pos;
in vec2 v_texcoord;

void main(void)
{

    vec4 color;
    float texelHSize = 1.0 / vec2(textureSize( target )).x;
    for( int i = 0; i < )
}
