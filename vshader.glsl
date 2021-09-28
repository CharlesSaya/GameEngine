#version 140

uniform mat4 mvp_matrix;

in vec4 a_position;
in vec2 a_texcoord;

out vec2 v_texcoord;

//! [0]
void main()
{
    // Calculate vertex position in screen space
    vec4 position = a_position;
    if ( a_position.x > 0.4 && a_position.x < 0.6 &&  a_position.y > 0.4 && a_position.y < 0.6 )
        position += vec4(0.0,0.0,-.10,0.0);
    gl_Position = mvp_matrix * position;

    // Pass texture coordinate to fragment shader
    // Value will be automatically interpolated to fragments inside polygon faces
    v_texcoord = a_texcoord;
}
//! [0]
