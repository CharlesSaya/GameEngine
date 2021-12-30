#version 330 core

uniform vec3 meshColor;

uniform sampler2D sphereTexture;
uniform sampler2D shadowTexture;

uniform vec3 lightPosition;
uniform vec3 cameraPosition;

in vec3 v_pos;
in vec3 v_normal;
in vec2 v_texcoord;
in vec4 v_lightSpacePos;

float shadowCalculation(vec4 fragPosLightSpace)
{
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;

    float closestDepth = texture(shadowTexture, projCoords.xy).r;
    float currentDepth = projCoords.z;

    vec3 normal = normalize(v_normal);
    vec3 lightDir = normalize(lightPosition - v_pos);
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);

    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowTexture, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowTexture, projCoords.xy + vec2(x, y) * texelSize).r;
            shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;
        }
    }
    shadow /= 9.0;

    if(projCoords.z > 1.0)
        shadow = 0.0;

    return shadow;
}

//! [0]
void main()
{
    vec4 color = texture( sphereTexture, v_texcoord );

    vec3 lightVector = normalize( lightPosition - v_pos );

    vec3 lambertian =  0.4 * max( 0.0, dot( lightVector, normalize(v_normal)  ) ) * vec3( 1.0 ) ;

    vec3 r = normalize( reflect ( -lightVector, normalize( v_normal ) ) );
    vec3 v = normalize( cameraPosition - v_pos );
    vec3 specular =  0.2 * pow( max( 0.0, dot( r, v ) ), 20.0 ) * vec3( 1.0 ) ;

    float shadow = shadowCalculation(v_lightSpacePos);

    vec3 ambient = 0.4 * color.xyz;

    gl_FragColor = vec4( ambient + ( 1.0 - shadow ) * ( lambertian +  specular ), 1.0);

}
//! [0]

