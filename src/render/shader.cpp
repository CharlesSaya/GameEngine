#include "headers/render/shader.h"

#include <iostream>
#include <fstream>

Shader::Shader(){

}

Shader::Shader( std::string vertexShader, std::string fragmentShader ){

    if (!program.addShaderFromSourceFile(QOpenGLShader::Vertex, vertexShader.c_str()))
        exit(0);

    if (!program.addShaderFromSourceFile(QOpenGLShader::Fragment, fragmentShader.c_str()))
        exit(0);

    if (!program.link())
        exit(0);

}

void Shader::loadDirectionalLight( DirectionalLight light ){
    this->useShaderProgram();
    this->setUniformValue( "directionalLight.direction", light.getDirection() );
    this->setUniformValue( "directionalLight.ambient", light.getAmbient() );
    this->setUniformValue( "directionalLight.diffuse", light.getDiffuse() );
    this->setUniformValue( "directionalLight.specular", light.getSpecular() );
}

void Shader::loadPointLights( std::vector<PointLight> lights ){
    this->useShaderProgram();

    for( uint i = 0; i < lights.size(); i++ ){
        this->setUniformValue( "pointLights["+std::to_string(i)+"].position", lights[i].getLightPosition() );

        this->setUniformValue( "pointLights["+std::to_string(i)+"].ambient", lights[i].getAmbient() );
        this->setUniformValue( "pointLights["+std::to_string(i)+"].diffuse", lights[i].getDiffuse() );
        this->setUniformValue( "pointLights["+std::to_string(i)+"].specular", lights[i].getSpecular() );

        this->setUniformValue( "pointLights["+std::to_string(i)+"].constant", lights[i].getConstant() );
        this->setUniformValue( "pointLights["+std::to_string(i)+"].linear", lights[i].getLinear() );
        this->setUniformValue( "pointLights["+std::to_string(i)+"].quadratic", lights[i].getQuadratic() );
    }
}

void Shader::useShaderProgram(){

    if (!program.bind())
        exit(0);
}


QOpenGLShaderProgram &Shader::getProgram()
{
    return program;
}

