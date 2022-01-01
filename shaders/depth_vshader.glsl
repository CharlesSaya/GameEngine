#version 330 core

in vec3 a_position;
in vec3 a_normal;
in vec2 a_texcoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main(){
    vec4 position = vec4( a_position, 1.0);

    gl_Position = projection * view * model * position;
}
