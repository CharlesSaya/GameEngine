#include "../../headers/render/BasicShader.h"


BasicShader::BasicShader(){

}

BasicShader::BasicShader( std::string vertexShader, std::string fragmentShader ){
    qDebug("%s",vertexShader.c_str());

    if (!program.addShaderFromSourceFile(QOpenGLShader::Vertex, vertexShader.c_str())){
        qDebug("%s", program.log().toStdString().c_str() );
    }
    qDebug("%s",fragmentShader.c_str());
    if (!program.addShaderFromSourceFile(QOpenGLShader::Fragment, fragmentShader.c_str()))
        exit(0);

    if (!program.link())
        exit(0);

}

void BasicShader::useShaderProgram(){

    if (!program.bind())
        exit(0);
}

BasicShader & BasicShader::getInstance(){
    static BasicShader instance( ":/shaders/base_vshader.glsl", ":/shaders/base_fshader.glsl" );
    return instance;
}

QOpenGLShaderProgram &BasicShader::getProgram()
{
    return program;
}

