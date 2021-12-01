#version 140

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform sampler2D heightMap;

in vec3 a_position;
in vec3 a_normal;
in vec2 a_texcoord;

out vec3 v_pos;
out vec3 v_normal;
out vec2 v_texcoord;
out float height;

//! [0]
void main()
{
    vec4 h = texture2D( heightMap,a_texcoord );
    vec4 position = vec4( a_position, 1.0);

    vec4 worldPosition = model * position;
    vec4 nWorld = model * vec4( a_normal, 0.0);

    gl_Position = projection * view * model * position;

    v_pos = vec4(model *  position).xyz ;
    v_texcoord  = a_texcoord;
    v_normal  = nWorld.xyz ;
    height = h.z;
}
//! [0]
