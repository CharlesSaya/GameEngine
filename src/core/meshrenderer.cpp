#include "headers/core/meshrenderer.h"

MeshRenderer::MeshRenderer(){

}

MeshRenderer::MeshRenderer(QOpenGLShaderProgram * program, Mesh &mesh){
    this->program = program;
    this->mesh = mesh;
}

void MeshRenderer::input(){

}

void MeshRenderer::update(){

}

void MeshRenderer::render(){

    this->program->setUniformValue("meshColor", this->mesh.getMeshColor() );
    this->mesh.drawMesh(this->program);

}
