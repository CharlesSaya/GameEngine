#include "headers/render/renderText.h"

RenderText::RenderText(){

}

RenderText::RenderText( Texture &fontAtlas ){
    this->context = QOpenGLContext::currentContext();
    this->glFuncs = this->context->versionFunctions<QOpenGLFunctions_3_3_Core>();

    this->fontAtlas = fontAtlas;
    this->initBuffers();
    this->initTexture();
}

void RenderText::initBuffers(){

    this->vBuffer = QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    this->vBuffer.create();

    this->tBuffer = QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    this->tBuffer.create();

}

void RenderText::initTexture(){

    glFuncs->glGenTextures(1, &textureID);
    glFuncs->glBindTexture(GL_TEXTURE_2D, textureID);
    glFuncs->glPixelStorei(GL_UNPACK_ALIGNMENT,1);

    glFuncs->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glFuncs->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glFuncs->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glFuncs->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glFuncs->glTexImage2D( GL_TEXTURE_2D , 0, GL_RGBA, 512, 512, 0, GL_RGBA, GL_UNSIGNED_BYTE, this->fontAtlas.getImage().constBits());
    glFuncs->glGenerateMipmap(GL_TEXTURE_2D);
}

void RenderText::renderText( std::string text, QVector2D position, QVector2D screenFormat, int size, Shader * shader  ){
    qDebug() << this->fontAtlas.getImage().width() << this->fontAtlas.getImage().height();
    uint length = text.length();

    float x = position.x();
    float y = position.y();

    std::vector<QVector2D> vertices;
    std::vector<QVector2D> texcoords;

    for ( uint i=0 ; i<length ; i++ ){

        QVector2D vertexDownLeft  = QVector2D( x+i*size     , y      );
        QVector2D vertexDownRight = QVector2D( x+i*size+size, y      );
        QVector2D vertexUpRight   = QVector2D( x+i*size+size, y+size );
        QVector2D vertexUpLeft    = QVector2D( x+i*size     , y+size );

        vertices.push_back( vertexDownLeft);
        vertices.push_back( vertexDownRight);
        vertices.push_back( vertexUpLeft  );

        vertices.push_back( vertexUpLeft   );
        vertices.push_back( vertexDownRight);
        vertices.push_back( vertexUpRight  );

        char character = text.at(i);
        float uvX = (character%16)/16.0f;
        float uvY = (character/16)/16.0f;

        QVector2D uvDownLeft    = QVector2D( uvX             , 1 - uvY );
        QVector2D uvDownRight   = QVector2D( uvX +1.0f/16.0f , 1 -  uvY );
        QVector2D uvUpLeft      = QVector2D( uvX +1.0f/16.0f , 1 - ( uvY + 1.0f/16.0f ) );
        QVector2D uvUpRight     = QVector2D( uvX             , 1 - ( uvY + 1.0f/16.0f ) );

        texcoords.push_back( uvDownLeft );
        texcoords.push_back( uvDownRight );
        texcoords.push_back( uvUpLeft  );

        texcoords.push_back( uvUpLeft   );
        texcoords.push_back( uvDownRight );
        texcoords.push_back( uvUpRight  );
    }

    shader->useShaderProgram();

    glFuncs->glActiveTexture(GL_TEXTURE0);
    glFuncs->glBindTexture(GL_TEXTURE_2D, textureID);
    shader->setUniformValue( "fontAtlas", 0 );
    shader->setUniformValue( "screenFormat", screenFormat );

    qDebug() << textureID;

    this->vBuffer.bind();
    this->vBuffer.allocate( vertices.data(), vertices.size() * sizeof( QVector2D) );

    int vertexLocation = shader->getProgram().attributeLocation("a_position");
    shader->getProgram().enableAttributeArray(vertexLocation);
    shader->getProgram().setAttributeBuffer(vertexLocation, GL_FLOAT, 0, 2, sizeof(QVector2D) );

    this->tBuffer.bind();
    this->tBuffer.allocate( texcoords.data(), texcoords.size() * sizeof( QVector2D) );

    int texcoordLocation = shader->getProgram().attributeLocation("a_texcoord");
    shader->getProgram().enableAttributeArray(texcoordLocation);
    shader->getProgram().setAttributeBuffer(texcoordLocation, GL_FLOAT, 0 , 2, sizeof(QVector2D) );

    glFuncs->glEnable(GL_BLEND);
    glFuncs->glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glFuncs->glDrawArrays(GL_TRIANGLES, 0, vertices.size() );

    glFuncs->glDisable(GL_BLEND);
}
