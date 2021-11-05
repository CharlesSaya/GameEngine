#version 140

uniform sampler2D rockTexture;
uniform sampler2D grassTexture;
uniform sampler2D snowTexture;
uniform vec3 meshColor;

in vec2 v_texcoord;
in float height;
//! [0]
void main()
{
    // Set fragment color from texture
    vec4 rockColor  = texture2D( rockTexture, v_texcoord);
    vec4 grassColor = texture2D( grassTexture, v_texcoord);
    vec4 snowColor  = texture2D( snowTexture, v_texcoord);

    if ( height <0.07)
        gl_FragColor = grassColor;
    else if(height>=0.07 && height < 0.3 )
        gl_FragColor = rockColor;
    else
        gl_FragColor = snowColor;

    gl_FragColor = vec4( meshColor, 1.0);

}
//! [0]

