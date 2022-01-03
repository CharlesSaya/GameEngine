#version 330 core

layout (location = 0) out vec4 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gDiffuse;
layout (location = 3) out vec4 gBloom;

in vec4 v_pos;
in vec3 v_normal;
in vec2 v_texcoord;

uniform sampler2D texture0;

void main(void)
{

    gPosition = v_pos;
    gNormal = normalize(v_normal);

    gDiffuse = texture( texture0, v_texcoord );

    float brightness = gDiffuse.r *  0.2126 + gDiffuse.g * 0.7152 + gDiffuse.b * 0.0722;

    if( brightness > 0.55 )
        gBloom = gDiffuse;
    else
        gBloom = vec4(0.0);
}
