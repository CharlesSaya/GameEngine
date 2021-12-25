#extension GL_ARB_bindless_texture : enable
uniform samplerCube skybox;

in vec2 texCoord;
in vec3 pos;

void main(void)
{
    gl_FragColor = textureCube( skybox, pos );
}
