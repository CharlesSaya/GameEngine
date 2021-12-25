#include "headers/render/texture.h"

Texture::Texture(){

}

Texture::Texture(  std::string file, std::string location ){
    this->image = QImage( file.c_str() ).mirrored();
    this->location = location;
    this->initTexture();
}


void Texture::initTexture(){
    this->texture = new QOpenGLTexture( this->image );

    this->texture->setWrapMode( QOpenGLTexture::Repeat );
    this->texture->setMinMagFilters( QOpenGLTexture::LinearMipMapNearest, QOpenGLTexture::LinearMipMapLinear );

    this->texture->generateMipMaps();
}

void Texture::bindTexture( uint textureUnit, Shader * shader ){

    this->texture->bind( textureUnit );
    shader->setUniformValue( this->location, textureUnit );
}

void Texture::unbindTexture(){
    this->texture->release();
}

QOpenGLTexture * Texture::getTexture()
{
    return this->texture;
}

QImage &Texture::getImage()
{
    return image;
}

void Texture::setImage(const QImage &newImage)
{
    image = newImage;
}
