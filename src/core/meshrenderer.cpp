#include "headers/core/meshrenderer.h"


MeshRenderer::MeshRenderer( Mesh &mesh, QObject * parent ) {
    this->mesh = mesh;
}

void MeshRenderer::renderMesh( Transform & transform, const QMatrix4x4& model, CameraComponent& camera) {

    float distance = ( camera.getCameraPosition() - transform.getWorldPosition()).length();

    Shader * shader = this->mesh.getShader();
    shader->useShaderProgram();

    shader->setUniformValue( "model", model );
    shader->setUniformValue( "view", camera.getViewMatrix() );
    shader->setUniformValue( "projection", camera.getProjection() );
    shader->setUniformValue( "cameraPosition", camera.getCameraPosition() );

    this->mesh.bindTextures();
    this->mesh.drawMesh( distance );

    if( this->mesh.getRenderAABB())
        this->mesh.drawAABB();

    this->mesh.unbindTextures();

}

void MeshRenderer::updateAABB( const QMatrix4x4& model )  {
    this->mesh.updateAABB( model );
}

Mesh &MeshRenderer::getMesh()
{
    return mesh;
}

void MeshRenderer::setMesh(const Mesh &newMesh)
{
    mesh = newMesh;
}
