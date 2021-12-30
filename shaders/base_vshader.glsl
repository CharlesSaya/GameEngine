#version 330 core

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightSpaceMatrix;

in vec3 a_position;
in vec3 a_normal;
in vec2 a_texcoord;

out vec3 v_pos;
out vec3 v_normal;
out vec2 v_texcoord;
out vec4 v_lightSpacePos;

//! [0]
void main()
{
    vec4 position = vec4( a_position, 1.0);

    vec4 worldPosition = model * position;

    //out
    v_pos           = worldPosition.xyz;
    v_texcoord      = a_texcoord;
    v_normal        = transpose(inverse(mat3(model))) *  a_normal;
    v_lightSpacePos = lightSpaceMatrix * worldPosition;

    gl_Position     = projection * view * worldPosition;

}
//! [0]
