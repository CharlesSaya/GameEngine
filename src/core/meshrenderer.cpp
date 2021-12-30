#include "headers/core/meshrenderer.h"


MeshRenderer::MeshRenderer( Mesh &mesh, QObject * parent ) {
    this->mesh = mesh;
}

void MeshRenderer::renderMesh( Transform & transform, const QMatrix4x4& model, CameraComponent * camera, Shader *shaderDepth ) {

    float distance = 0.0;

    if(shaderDepth != nullptr)
        shader = shaderDepth;
    else{
        shader = this->mesh.getShader();
        distance = (camera->getCameraPosition() - transform.getWorldPosition()).length();
    }

    shader->useShaderProgram();
    shader->setUniformValue( "model", model );
    shader->setUniformValue( "view", camera->getViewMatrix());
    shader->setUniformValue( "projection", camera->getProjection() );
    shader->setUniformValue( "cameraPosition", camera->getCameraPosition() );
    shader->setUniformValue( "lightSpaceMatrix", lightSpaceMatrix );

    this->mesh.bindTextures();
    this->mesh.drawMesh( distance );

    if( this->mesh.getRenderAABB() )
        this->mesh.drawAABB();

    this->mesh.unbindTextures();
   }


void MeshRenderer::updateAABB( const QMatrix4x4& model )  {
    this->mesh.updateAABB( model );
}

void MeshRenderer::setLightSpaceMatrix(const QMatrix4x4 & lightSpaceMatrix){
    this->lightSpaceMatrix = lightSpaceMatrix;
}

Mesh &MeshRenderer::getMesh()
{
    return mesh;
}

void MeshRenderer::setMesh(const Mesh &newMesh)
{
    mesh = newMesh;
}
