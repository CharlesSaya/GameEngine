#version 330 core

struct DirectionalLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform vec3 cameraPosition;

uniform sampler2D diffuseTexture;
uniform sampler2D shadowTexture;
uniform sampler2D normalTexture;
uniform sampler2D positionTexture;

uniform mat4 lightSpaceMatrix;

uniform PointLight pointLights[1];
uniform DirectionalLight directionalLight;

in vec2 v_texcoord;


float shadowCalculation(vec4 fragPosLightSpace, vec3 normal)
{
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;

    float closestDepth = texture(shadowTexture, projCoords.xy).r;
    float currentDepth = projCoords.z;

    float bias = max(0.05 * (1.0 - dot(normal, normalize(-directionalLight.direction))), 0.005);

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

// calculates the color when using a directional light.
vec3 CalcDirLight(DirectionalLight light, vec3 normal, vec3 pos, vec4 lightSpacePos )
{

    vec4 color = texture( diffuseTexture, v_texcoord );

    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 lambertian =  0.4 * max( 0.0, dot( lightDir, normal  ) ) * vec3( 1.0 ) ;

    vec3 r = normalize( reflect ( -lightDir, normal ) );
    vec3 v = normalize( cameraPosition - pos );
    vec3 specular =  0.2 * pow( max( 0.0, dot( r, v ) ), 20.0 ) * vec3( 1.0 ) ;
    vec3 ambient = 0.4 * color.xyz;

    float shadow = shadowCalculation(lightSpacePos, normal);

    return  ambient + ( 1.0 - shadow ) * ( lambertian +  specular );
}

// calculates the color when using a point light.
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 pos, vec4 lightSpacePos )
{
    vec4 color = texture( diffuseTexture, v_texcoord );

    vec3 lightDir = normalize(light.position - pos);

    // specular shading

    float distance = length(light.position - pos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    // combine results

    vec3 lambertian =  0.4 * max( 0.0, dot( lightDir, normalize(normal)  ) ) * vec3( 1.0 ) ;

    vec3 r = normalize( reflect ( -lightDir, normalize( normal ) ) );
    vec3 v = normalize( cameraPosition - pos );
    vec3 specular =  0.2 * pow( max( 0.0, dot( r, v ) ), 20.0 ) * vec3( 1.0 ) ;
    vec3 ambient = 0.4 * color.xyz;

    ambient *= attenuation;
    lambertian *= attenuation;
    specular *= attenuation;

    float shadow = shadowCalculation(lightSpacePos, normal);

    return  ambient + ( 1.0 - shadow ) * ( lambertian +  specular );
}

//! [0]
void main()
{
    vec3 normal        = normalize(texture( normalTexture, v_texcoord.xy ).rgb);
    vec3 pos           = texture( positionTexture, v_texcoord.xy ).rgb;
    vec4 lightSpacePos = lightSpaceMatrix * texture( positionTexture, v_texcoord.xy );
    vec3 viewDir = normalize( cameraPosition - pos);

    vec3 color = CalcDirLight( directionalLight, normal, pos, lightSpacePos );
    for(int i = 0; i < 1; i++)
        color += CalcPointLight( pointLights[i], normal, pos, lightSpacePos);
    vec4 a = texture( diffuseTexture, v_texcoord );

    gl_FragColor = vec4( color , 1.0 );

}
//! [0]

