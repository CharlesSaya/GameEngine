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

void Shader::useShaderProgram(){

    if (!program.bind())
        exit(0);
}


QOpenGLShaderProgram &Shader::getProgram()
{
    return program;
}

