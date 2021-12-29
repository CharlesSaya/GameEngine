#version 140

in vec3 a_position;
in vec3 a_normal;
in vec2 a_texcoord;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

void main(){
    vec4 position = vec4( a_position, 1.0);

    vec4 worldPosition = model * position;
    vec4 nWorld = model * vec4( a_normal, 0.0);

    gl_Position =projection * view * model * position;
}
