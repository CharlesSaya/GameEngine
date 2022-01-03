#version 330 core

uniform sampler2D target;

in vec2 blurTexcoords[5];

void main(void)
{
    vec4 color;
    color += texture( target, blurTexcoords[0] ) * 0.06136;
    color += texture( target, blurTexcoords[1] ) * 0.24477;
    color += texture( target, blurTexcoords[2] ) * 0.38774;
    color += texture( target, blurTexcoords[3] ) * 0.24477;
    color += texture( target, blurTexcoords[4] ) * 0.06136;

    gl_FragColor = color ;
}
