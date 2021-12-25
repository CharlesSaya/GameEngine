#version 140

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

in vec3 a_position;
in vec3 a_normal;
in vec2 a_texcoord;

out vec2 texCoord;
out vec3 pos;

void main()
{
    pos = a_position;
    texCoord = a_texcoord;

    gl_Position = projection * view * model * vec4( a_position, 1.0 );
}
