#version 330 core

in vec3 a_position;
in vec2 a_texcoord;

uniform sampler2D target;

out vec2 blurTexcoords[5];

void main(){

    float texelHSize = 1 / vec2( textureSize( target, 0 ) ).x;
    for( int i = -2 ; i <= 2 ; i++ )
        blurTexcoords[i] = a_texcoord + i * vec2( texelHSize, 0.0 );

    gl_Position = vec4( a_position, 1.0 );
}
