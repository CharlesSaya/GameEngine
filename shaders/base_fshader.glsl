uniform sampler2D sphereTexture;

uniform vec3 meshColor;

uniform vec3 lightPosition;
uniform vec3 cameraPosition;

in vec3 v_pos;
in vec3 v_normal;
in vec2 v_texcoord;

//! [0]
void main()
{
    vec4 color = texture2D( sphereTexture, v_texcoord );

    vec3 lightVector = normalize( lightPosition - v_pos );

    vec3 lambertian =  0.4 * max( 0.0, dot( lightVector, normalize(v_normal)  ) ) * vec3( 1.0 ) ;

    vec3 r = normalize( reflect ( -lightVector, normalize( v_normal ) ) );
    vec3 v = normalize( cameraPosition - v_pos );
    vec3 specular =  0.2 * pow( max( 0.0, dot( r, v ) ), 20.0 ) * vec3( 1.0 ) ;

    vec3 ambient = 0.4 * color.xyz;

    gl_FragColor = vec4( ambient + lambertian +  specular , 1.0);

}
//! [0]

