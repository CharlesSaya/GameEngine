#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <QOpenGLShaderProgram>

#include "headers/render/pointLight.h"
#include "headers/render/directionalLight.h"

class Shader{

public:

    Shader();
    Shader( std::string vertexShader, std::string fragmentShader );

    void useShaderProgram();
    void updateUniforms();

    void loadDirectionalLight( DirectionalLight light );
    void loadPointLights( std::vector<PointLight> lights );

    QOpenGLShaderProgram &getProgram();

    template<typename T> void setUniformValue( std::string location, T value );


private:
    QOpenGLShaderProgram program;

};

template<typename T> inline void Shader::setUniformValue( std::string location, T value ){
    this->program.setUniformValue( location.c_str(), value );
}

#endif // SHADER_H
