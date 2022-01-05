#include "headers/render/texture.h"

Texture::Texture(){

}

Texture::Texture(  std::string file, std::string location ){
    this->image = QImage( file.c_str() ).mirrored();
    this->location = location;
    this->initTexture();
    this->context = QOpenGLContext::currentContext();
    glFuncs = this->context->versionFunctions<QOpenGLFunctions_3_3_Core>();
}


void Texture::initTexture(){
    this->texture = new QOpenGLTexture( this->image );
    this->texture->setWrapMode( QOpenGLTexture::Repeat );
    this->texture->setMinMagFilters( QOpenGLTexture::LinearMipMapNearest, QOpenGLTexture::LinearMipMapLinear );

    this->texture->generateMipMaps();
}

void Texture::bindTexture( uint textureUnit, Shader * shader ) const {

    glFuncs->glActiveTexture( GL_TEXTURE0 + textureUnit );
    this->texture->bind();
//    qDebug() <<  GL_TEXTURE0 + textureUnit  << this->location.c_str();
    shader->setUniformValue( this->location, textureUnit );

}

void Texture::unbindTexture( uint textureUnit ) const{
    glFuncs->glActiveTexture( GL_TEXTURE0 + textureUnit );
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

const std::string &Texture::getLocation() const
{
    return location;
}

void Texture::setLocation(const std::string &newLocation)
{
    location = newLocation;
}
