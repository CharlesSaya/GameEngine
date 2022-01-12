#version 330 core

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec2 a_texCoord;
layout (location = 3) in vec4 xyzs;
layout (location = 4) in vec4 color;

uniform mat4 view;
uniform mat4 projection;

out vec2 v_texcoord;
out vec4 v_color;

void main()
{
    float scale = 10.0f;
    v_texcoord = a_position.xy + vec2(0.5, 0.5);
    v_color    = color;

    float particleSize = xyzs.w;

    vec3 CameraRight = vec3(view[0][0], view[1][0], view[2][0]);
    vec3 CameraUp = vec3(view[0][1], view[1][1], view[2][1]);

    vec3 particleCenter_wordspace = xyzs.xyz;

    vec3 vertexPosition_worldspace =
                    particleCenter_wordspace
                    + CameraRight * a_position.x * particleSize
                    + CameraUp * a_position.y * particleSize;

    gl_Position = projection * view * vec4(vertexPosition_worldspace, 1.0f);

}
