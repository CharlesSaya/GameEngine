#version 330 core

layout (location = 0) out vec4 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gDiffuse;

in vec4 v_pos;
in vec3 v_normal;
in vec2 v_texcoord;

uniform sampler2D texture0;

void main(void)
{

    gPosition = v_pos;
    gNormal = normalize(v_normal);

    gDiffuse = texture( texture0, v_texcoord );

}
