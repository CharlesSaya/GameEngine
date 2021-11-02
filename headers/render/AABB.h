#ifndef AABB_H
#define AABB_H

#include<QOpenGLFunctions_3_1>
#include<QOpenGLShaderProgram>
#include<QOpenGLBuffer>
#include<QVector3D>
#include<vector>

class AABB{

    private:
        int indexCount;

        std::vector<QVector3D> vertices;
        std::vector<GLushort> lines ;

        QOpenGLBuffer verticesBuffer, indexesBuffer;
        QVector3D min = QVector3D(__FLT_MAX__ ,__FLT_MAX__, __FLT_MAX__) ;
        QVector3D max = QVector3D(-__FLT_MAX__ ,-__FLT_MAX__,-__FLT_MAX__) ;

    public:

        AABB();

        AABB( std::vector<QVector3D> points );

        const QVector3D &getMin() const;
        void setMin(const QVector3D &newMin);

        const QVector3D &getMax() const;
        void setMax(const QVector3D &newMax);

        void initBuffers();
        void drawAABB( QOpenGLShaderProgram * program );

};




#endif // AABB_H
