#version 330 core

layout (location = 0) out vec4 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gDiffuse;
layout (location = 3) out vec4 gBloom;

in vec4 v_pos;
in vec3 v_normal;
in vec2 v_texcoord;

uniform sampler2D blendmapTop;
uniform sampler2D blendmapMiddle;
uniform sampler2D blendmapBottom;

uniform sampler2D snowTexture;
uniform sampler2D rockTexture;
uniform sampler2D grassTexture;
uniform sampler2D plateauTexture;
uniform sampler2D sandTexture;
uniform sampler2D hillTexture;


uniform sampler2D texture0;

void main(void)
{


//    vec4 blendMapTopColour = texture(blendmapBottom, v_texcoord);
//    vec4 blendMapMiddleColour = texture(blendmapMiddle, v_texcoord);
//    vec4 blendMapBottomColour = texture(blendmapBottom, v_texcoord);


//    float backTextureAmount = 1 - (blendMapTopColour.r + blendMapMiddleColour.r + blendMapBottomColour.r +
//                                   blendMapTopColour.g + blendMapMiddleColour.g + blendMapBottomColour.g );
//    vec2 tiledCoords = v_texcoord * 40.0;
//    vec4 backgroundTextureColour = texture(grassTexture,tiledCoords) * backTextureAmount +
//            texture(sandTexture,tiledCoords) * backTextureAmount +
//            texture(rockTexture,tiledCoords) * backTextureAmount;

//    vec4 rTextureColour = texture(hillTexture, tiledCoords)* blendMapTopColour.r +
//            texture(plateauTexture, tiledCoords)* blendMapTopColour.r+
//            texture(snowTexture, tiledCoords)* blendMapTopColour.r;

    //TODO PATH

//    vec4 totalColour = backgroundTextureColour + rTextureColour ;


    gPosition = v_pos;
    gNormal = normalize(v_normal);

    gDiffuse = texture( blendmapBottom, v_texcoord );

    float brightness = gDiffuse.r *  0.2126 + gDiffuse.g * 0.7152 + gDiffuse.b * 0.0722;

    if( brightness > 0.55 )
        gBloom = gDiffuse;
    else
        gBloom = vec4(0.0);
}
