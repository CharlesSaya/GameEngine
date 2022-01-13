#version 330 core

struct DirectionalLight {
    vec3 direction;

    vec3 lightColor;
};

struct PointLight {
    vec3 position;

    vec3 lightColor;

    float constant;
    float linear;
    float quadratic;
};

uniform vec3 cameraPosition;

uniform sampler2D diffuseTexture;
uniform sampler2D shadowTexture;
uniform sampler2D normalTexture;
uniform sampler2D positionTexture;
uniform sampler2D bloomTexture;
uniform sampler2D lightSpacePosTexture;

uniform mat4 lightSpaceMatrix;
uniform mat4 view;

uniform PointLight pointLights[1];
uniform DirectionalLight directionalLight;

in vec2 v_texcoord;

float shadowCalculation(vec4 fragPosLightSpace, vec3 normal)
{
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;

    float currentDepth = projCoords.z;

    float bias = max(0.05 * (1.0 - dot(normal, normalize(-directionalLight.direction))), 0.005);
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowTexture, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcf = texture(shadowTexture, projCoords.xy + vec2(x, y) * texelSize).r;
            shadow += currentDepth - bias > pcf  ? 1.0 : 0.0;
        }
    }
    shadow /= 9.0;

    if(projCoords.z > 1.0)
        shadow = 0.0;

    return shadow;

}

vec3 colorDirectionalLight(DirectionalLight light, vec3 normal, vec3 pos, vec4 lightSpacePos )
{

    vec4 color = texture( diffuseTexture, v_texcoord );

    vec3 lightDir = normalize(-light.direction);

    float diff = max(dot(normal, lightDir), 0.0);

    vec3 lambertian =  0.4 * max( 0.0, dot( lightDir, normal  ) ) * light.lightColor ;

    vec3 r = normalize( reflect ( -lightDir, normal ) );
    vec3 v = normalize( cameraPosition - pos );
    vec3 specular =  0.2 * pow( max( 0.0, dot( r, v ) ), 20.0 ) * light.lightColor ;

    vec3 ambient = 0.4 * color.xyz * light.lightColor;

    float shadow = shadowCalculation( lightSpacePos, normal);

    return  ambient +  ( 1.0 -  shadow )* ( lambertian  );

}

vec3 colorPointLight(PointLight light, vec3 normal, vec3 pos, vec4 lightSpacePos )
{
    vec4 color = texture( diffuseTexture, v_texcoord );

    vec3 lightDir = normalize(light.position - pos);

    float distance = length(light.position - pos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    vec3 lambertian =  0.4 * max( 0.0, dot( lightDir, normalize(normal)  ) ) * light.lightColor ;

    vec3 r = normalize( reflect ( -lightDir, normalize( normal ) ) );
    vec3 v = normalize( cameraPosition - pos );
    vec3 specular =  0.2 * pow( max( 0.0, dot( r, v ) ), 20.0 ) * light.lightColor ;

    vec3 ambient  = 0.4 * color.xyz * light.lightColor;

    ambient    *= attenuation;
    lambertian *= attenuation;
    specular   *= attenuation;

    float shadow = shadowCalculation(lightSpacePos, normal);

    return  ambient + ( 1.0 - shadow ) * ( lambertian +  specular );
}

//! [0]
void main()
{


    vec4 pos           = texture( positionTexture, v_texcoord.xy );
    vec4 lightSpacePos = lightSpaceMatrix * pos;
//    vec4 lightSpacePos = texture( lightSpacePosTexture, v_texcoord.xy );
    vec3 viewDir       = normalize( cameraPosition - pos.rgb);
    vec3 normal        = normalize( texture( normalTexture, v_texcoord.xy ).rgb);

    vec3 color = colorDirectionalLight( directionalLight, normal, pos.rgb, lightSpacePos );


//    for(int i = 0; i < 1; i++)
//        color += colorPointLight( pointLights[i], normal, pos.rgb, lightSpacePos);

    gl_FragColor = vec4( color , 1.0 );
//    gl_FragColor = lightSpacePos;

}
//! [0]

