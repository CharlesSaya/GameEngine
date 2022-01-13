#version 330 core

layout (location = 0) out vec4 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gDiffuse;
layout (location = 3) out vec4 gBloom;

in vec4 v_pos;
in vec4 v_viewPos;
in vec3 v_normal;
in vec2 v_texcoord;
in vec4 v_lightSpacePos;

uniform bool terrain;

uniform sampler2D tex0;
uniform sampler2D tex1;
uniform sampler2D tex2;
uniform sampler2D tex3;
uniform sampler2D tex4;
uniform sampler2D tex5;

uniform sampler2D blendmapTop;      // blendmap top
uniform sampler2D blendmapMiddle; // blendmap middle
uniform sampler2D blendmapBottom; // blendmap bottom

void main(void)
{

    vec4 blendMapTopColour    = texture(blendmapTop, v_texcoord);
    vec4 blendMapMiddleColour = texture(blendmapMiddle, v_texcoord);
    vec4 blendMapBottomColour = texture(blendmapBottom, v_texcoord);

    float backTextureAmount = 1 - (blendMapTopColour.r +
                                   blendMapTopColour.g +
                                   blendMapTopColour.b );

    float backTextureAmountMiddle = 1 - (blendMapMiddleColour.r +
                                      blendMapMiddleColour.g +
                                      blendMapMiddleColour.b );

    float backTextureAmountBottom = 1 - ( blendMapBottomColour.r +
                                       blendMapBottomColour.g +
                                           blendMapBottomColour.b );

    vec4 rTextureColour = texture(tex0, v_texcoord) * blendMapTopColour.r +
                          texture(tex3, v_texcoord) * blendMapMiddleColour.r+
                          texture(tex5, v_texcoord) * blendMapBottomColour.r;

    vec4 gTextureColour = vec4( 0.75) * blendMapTopColour.g +
                          vec4( 0.75) * blendMapMiddleColour.g+
                          vec4( 0.75) * blendMapBottomColour.g;

    vec4 bTextureColour = texture(tex1, v_texcoord) * blendMapTopColour.b +
                          texture(tex4, v_texcoord) * blendMapMiddleColour.b+
                          texture(tex2, v_texcoord) * blendMapBottomColour.b;


    vec4 totalColour = rTextureColour + gTextureColour + bTextureColour;

    gPosition = v_pos;

    vec3 xTangent = vec4(dFdx( v_viewPos )).xyz;
    vec3 yTangent = vec4(dFdy( v_viewPos )).xyz;
    vec3 faceNormal = normalize( cross( xTangent, yTangent ) );

    gNormal = normalize(faceNormal);

    if( terrain )
        gDiffuse = totalColour ;
    else
        gDiffuse =  texture(tex0, v_texcoord) ;

    float brightness = gDiffuse.r *  0.2126 + gDiffuse.g * 0.7152 + gDiffuse.b * 0.0722;

    if( brightness > 0.75 )
        gBloom = gDiffuse;
    else
        gBloom = vec4(0.0);
}
