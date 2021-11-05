#ifndef BASICSHADER_H
#define BASICSHADER_H

#include <string>
#include <QOpenGLShaderProgram>

class BasicShader{

public:

    void useShaderProgram();
    void updateUniforms();

    static BasicShader & getInstance();


    QOpenGLShaderProgram &getProgram();
    void setProgram(const QOpenGLShaderProgram &newProgram);

private:
    QOpenGLShaderProgram program;

    BasicShader();
    BasicShader( std::string vertexShader, std::string fragmentShader );
};

#endif // BASICSHADER_H
