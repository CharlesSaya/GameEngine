#version 330 core

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec2 a_texcoord;
layout (location = 3) in vec4 xysc;

uniform vec3 cameraRight_W;
uniform vec3 cameraUp_W;

uniform mat4 view;
uniform mat4 projection;


out vec2 v_texcoord;

void main()
{

    v_texcoord = a_texcoord;

    vec2 screenPosition = a_position.xy * xysc.zw + xysc.xy;

    screenPosition.x = screenPosition.x * 2.0 - 1.0;
    screenPosition.y = screenPosition.y * -2.0 + 1.0;
    gl_Position = vec4(screenPosition, 0.0, 1.0);
}
