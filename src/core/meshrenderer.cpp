#include "headers/core/meshrenderer.h"

/**
 * @brief Constructeur
 * @param mesh
 * @param parent
 */

MeshRenderer::MeshRenderer( Mesh &mesh, QObject * parent ) {
    this->mesh = mesh;
}

/**
 * @brief Dessine le mesh associé au MeshRenderer
 * @param transform
 * @param model
 * @param camera
 * @param shader
 */

void MeshRenderer::renderMesh( Transform & transform, const QMatrix4x4& model, CameraComponent * camera, Shader * shader ) {

    float distance = 0.0;

    if(shader != nullptr)
        shader = shader;
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

    shader->setUniformValue( "terrain", this->mesh.isTerrain() );

    this->mesh.bindTextures( shader );

    this->mesh.drawMesh( distance, shader );
//    this->mesh.drawAABB( shader );

    this->mesh.unbindTextures();
   }

/**
 * @brief Mets à jour la BoundingBox du mesh
 * @param model
 */

void MeshRenderer::updateAABB( const QMatrix4x4& model )  {
    this->mesh.updateAABB( model );
}

// Getters & Setters

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
