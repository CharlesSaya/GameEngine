#include "headers/core/meshrenderer.h"

MeshRenderer::MeshRenderer(){

}

MeshRenderer::MeshRenderer(QOpenGLShaderProgram * program, Mesh &mesh){
    this->program = program;
    this->mesh = mesh;
}

void MeshRenderer::render(){
    if( renderAABB )
        qDebug( "Implement AABB rendering" );
}

