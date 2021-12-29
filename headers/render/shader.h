#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <QOpenGLShaderProgram>


class Shader{

public:

    Shader();
    Shader( std::string vertexShader, std::string fragmentShader );

    void useShaderProgram();
    void updateUniforms();
    bool isEmpty();

    QOpenGLShaderProgram &getProgram();

    template<typename T> void setUniformValue( std::string location, T value );


private:
    QOpenGLShaderProgram program;
    bool empty = true;

};

template<typename T> inline void Shader::setUniformValue( std::string location, T value ){
    this->program.setUniformValue( location.c_str(), value );
}

#endif // SHADER_H
