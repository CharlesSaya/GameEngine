#version 140

uniform mat4 mvp_matrix;
uniform sampler2D heightMap;
in vec4 a_position;
in vec2 a_texcoord;

out vec2 v_texcoord;
out float height;

//! [0]
void main()
{
    // Calculate vertex position in screen space
    vec4 position = a_position;
    vec4 h = texture2D( heightMap,a_texcoord );
//    position += vec4(0.,0., - h.z*1.8,0.);
    gl_Position = mvp_matrix * position;

    // Pass texture coordinate to fragment shader
    // Value will be automatically interpolated to fragments inside polygon faces
    v_texcoord = a_texcoord;
    height = h.z;
}
//! [0]
