#include "headers/render/flareGenerator.h"

void FlareGenerator::setCamera(GameObjectCamera *newCamera)
{
    camera = newCamera;
}

FlareGenerator::FlareGenerator(){

}

FlareGenerator::FlareGenerator(const DirectionalLight &sun, const std::vector<FlareTexture> &textures ) : sun(sun),
    flareTextures(textures)
{
    this->context = QOpenGLContext::currentContext();
    glFuncs = this->context->versionFunctions<QOpenGLFunctions_3_3_Core>();
    initBuffers();
    initGeometry();
}

void FlareGenerator::initBuffers(){

    this->vBuffer = QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    this->vBuffer.create();

    this->positionsBuffer = QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    this->positionsBuffer.create();

}

void FlareGenerator::initGeometry(){

    particleData.push_back( { QVector3D( -1.0f,  1.0f, 0.0f ), QVector3D( 0.0f,  0.0f, 1.0f ), QVector2D( 0.0f, 1.0f ) });
    particleData.push_back( { QVector3D( -1.0f, -1.0f, 0.0f ), QVector3D( 0.0f,  0.0f, 1.0f ), QVector2D( 0.0f, 0.0f ) });
    particleData.push_back( { QVector3D(  1.0f,  1.0f, 0.0f ), QVector3D( 0.0f,  0.0f, 1.0f ), QVector2D( 1.0f, 1.0f ) });
    particleData.push_back( { QVector3D(  1.0f, -1.0f, 0.0f ), QVector3D( 0.0f,  0.0f, 1.0f ), QVector2D( 1.0f, 0.0f ) });

    this->vBuffer.bind();
    this->vBuffer.allocate(particleData.data(), 4 * sizeof(VertexData) );

}

QVector2D FlareGenerator::sunScreenPosition(){
    QVector4D sunPosition = camera->getCameraComponent()->getProjection() * camera->getCameraComponent()->getViewMatrix() * QVector4D( sun.getLightPosition(), 1.0 );

    if( sunPosition.w() <= 0.)
        return QVector2D( -__FLT_MAX__, -__FLT_MAX__);
    float x = ( sunPosition.x() / sunPosition.w() + 1.0 ) / 2. ;
    float y = 1.0 - ( sunPosition.y() / sunPosition.w() + 1.0 ) / 2. ;
    return QVector2D( x, y );
}

void FlareGenerator::computeFlaresPosition( QVector2D sunToCenter, QVector2D sunPosition ){
    for( uint i = 0 ; i < flareTextures.size(); i++ ){
        flareTextures[i].setScreenPos( QVector2D( sunPosition + i *  sunToCenter ) );
    }
}

void FlareGenerator::renderFlare( FlareTexture flare, Shader * shader ){
    flare.getTexture().bindTexture( 0, shader );

    float xScale = flare.getScale();
    float yScale = xScale * (float)width / (float)height;
    QVector2D screenPos = flare.getScreenPos();

    positions.clear();
    positions.push_back( QVector4D( screenPos, xScale, yScale ) );

    quintptr offset = 0;

    this->vBuffer.bind();

    int vertexLocation = shader->getProgram().attributeLocation("a_position");
    shader->getProgram().enableAttributeArray(vertexLocation);
    shader->getProgram().setAttributeBuffer(vertexLocation, GL_FLOAT, offset, 3, sizeof(VertexData));

    offset += sizeof(QVector3D);

    int normalLocation = shader->getProgram().attributeLocation("a_normal");
    shader->getProgram().enableAttributeArray(normalLocation);
    shader->getProgram().setAttributeBuffer(normalLocation, GL_FLOAT, offset , 3, sizeof(VertexData));

    offset += sizeof(QVector3D);

    int texcoordLocation = shader->getProgram().attributeLocation("a_texcoord");
    shader->getProgram().enableAttributeArray(texcoordLocation);
    shader->getProgram().setAttributeBuffer(texcoordLocation, GL_FLOAT, offset , 2, sizeof(VertexData));

    positionsBuffer.bind();
    positionsBuffer.allocate( positions.data(), sizeof(QVector4D) );

    int xyzsLocation = shader->getProgram().attributeLocation("xysc");
    shader->getProgram().enableAttributeArray(xyzsLocation);
    shader->getProgram().setAttributeBuffer(xyzsLocation, GL_FLOAT, 0 , 4, sizeof(QVector4D));


    glFuncs->glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 );
}

void FlareGenerator::render( float brightness, Shader * shader ){

    QMatrix4x4 view = camera->getCameraComponent()->getViewMatrix();
    QVector3D rightW = QVector3D( view.column(0).x(), view.column(0).y(),view.column(0).z() );
    QVector3D up = QVector3D( view.column(0).y(), view.column(1).y(),view.column(2).y() );

    QVector2D sunPosition = sunScreenPosition();

    if( sunPosition.x() == -__FLT_MAX__)
        return;

    QVector2D sunToCenter = QVector2D( 0.5, 0.5 ) - sunPosition;
    float relativeBrightness = 1.0f - ( sunToCenter.length() / 0.7f );

    if( relativeBrightness < 0.0f)
        return;

    computeFlaresPosition( sunToCenter, sunPosition );

    shader->useShaderProgram();


    shader->setUniformValue( "cameraRight_W", rightW);
    shader->setUniformValue( "cameraUp_W", up );
    shader->setUniformValue( "view", view );
    shader->setUniformValue( "projection", camera->getCameraComponent()->getProjection() );

    shader->setUniformValue("brightness", 1.0f);

    renderFlare( flareTextures[0], shader );
    shader->setUniformValue("brightness", relativeBrightness);

    for( uint i = 1; i< flareTextures.size(); i++ )
        renderFlare( flareTextures[i], shader );
}

void FlareGenerator::screenResized( int width, int height  ){
    this->height = height;
    this->width = width;

}

