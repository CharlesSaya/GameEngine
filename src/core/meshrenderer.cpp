#include "headers/core/meshrenderer.h"


MeshRenderer::MeshRenderer( Mesh &mesh ) {
    this->mesh = mesh;
}

void MeshRenderer::renderMesh( Transform & transform, const QMatrix4x4& model, Camera& camera) {

    float distance = ( camera.getCameraPosition() - transform.getWorldPosition()).length();

    Shader * shader = this->mesh.getShader();
    shader->useShaderProgram();

    shader->setUniformValue( "model", model );
    shader->setUniformValue( "view", camera.getViewMatrix() );
    shader->setUniformValue( "projection", camera.getProjection() );
    shader->setUniformValue( "cameraPosition", camera.getCameraPosition() );

    this->mesh.bindTextures();
    this->mesh.drawMesh( distance );
    this->mesh.unbindTextures();

}

void MeshRenderer::updateBBox( Transform * transform ){
    this->mesh.updateBBox( transform );
}

Mesh &MeshRenderer::getMesh()
{
    return mesh;
}

void MeshRenderer::setMesh(const Mesh &newMesh)
{
    mesh = newMesh;
}
