#include "headers/core/meshrenderer.h"


MeshRenderer::MeshRenderer( Mesh &_mesh,Transform & _transform ) :  transform(_transform) {
    this->mesh = _mesh;
}

bool MeshRenderer::eventFilter( QObject * obj,  QEvent * event ){

    return false;
}

void MeshRenderer::input( QKeyEvent * key  ){

}

void MeshRenderer::update(){

}

void MeshRenderer::render( const QMatrix4x4& model, const QMatrix4x4& view, const QMatrix4x4& projection, const QVector3D& cameraPosition ) {

    float distance = ( cameraPosition - this->transform.getWorldPosition()).length();

    Shader * shader = this->mesh.getShader();
    shader->useShaderProgram();

    shader->setUniformValue( "model", model );
    shader->setUniformValue( "view", view );
    shader->setUniformValue( "projection", projection );
    shader->setUniformValue( "cameraPosition", cameraPosition );

    this->mesh.bindTextures();
    this->mesh.drawMesh( distance );
    this->mesh.unbindTextures();

}
