#version 330 core

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightSpaceMatrix;

uniform sampler2D heightMap;

in vec3 a_position;
in vec3 a_normal;
in vec2 a_texcoord;

out vec3 v_pos;
out vec3 viewPos;
flat out vec3 v_normal;
out vec2 v_texcoord;
out vec4 v_lightSpacePos;

out float height;

//! [0]
void main()
{
    float h = texture( heightMap,  a_texcoord ).r * 10.;

    vec4 position = vec4( a_position, 1.0) + vec4( 0.0, h , 0.0,  0.0 ) ;
    vec4 worldPosition = model * position;

    //out
    v_pos           = worldPosition.xyz;
    viewPos         = vec4(view * worldPosition).xyz;
    v_normal        = transpose(inverse(mat3(model))) *  a_normal;
    v_texcoord      = a_texcoord;
    v_lightSpacePos = lightSpaceMatrix * worldPosition;
    height          = h;

    gl_Position     = projection * view * worldPosition;

}
//! [0]
