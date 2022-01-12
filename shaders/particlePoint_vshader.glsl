#version 330 core

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec2 a_texCoord;
layout (location = 3) in vec4 xyzs;
layout (location = 4) in vec4 color;


uniform vec3 cameraRight_W;
uniform vec3 cameraUp_W;

uniform mat4 world;
uniform mat4 view ;
uniform mat4 projection;

out vec2 v_texcoord;
out vec4 v_color;

void main()
{
    v_texcoord = a_position.xy + vec2(0.5, 0.5);
    v_color    = color;

    float particleSize = xyzs.w;
    vec3 offset = xyzs.xyz;
    gl_Position = projection * view * mat4(1) * vec4(a_position + offset, 1.0f);
    gl_PointSize = particleSize;
}
