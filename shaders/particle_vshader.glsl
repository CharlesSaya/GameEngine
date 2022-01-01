#version 330 core

in vec3 a_position;
in vec3 a_normal;
in vec2 a_texCoord;

in vec4 xyzs;
in vec4 color;

uniform vec3 cameraRight_W;
uniform vec3 cameraUp_W;

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
    vec3 particleCenter_wordspace = xyzs.xyz;

    vec3 vertexPosition_worldspace =
                    particleCenter_wordspace
                    + cameraRight_W * a_position.x * particleSize
                    + cameraUp_W * a_position.y * particleSize;

    gl_Position = projection * view * vec4(vertexPosition_worldspace, 1.0f);

}
