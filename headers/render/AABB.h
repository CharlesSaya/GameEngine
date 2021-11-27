#ifndef AABB_H
#define AABB_H


#include<vector>

#include<QVector3D>
#include<QOpenGLBuffer>
#include<QOpenGLFunctions_3_1>

#include "headers/render/shader.h"
#include "headers/render/vertexData.h"

class AABB {

    private:
        int indexCount;

        std::vector<QVector3D> vertices;
        std::vector<GLuint> lines ;

        QOpenGLBuffer verticesBuffer, indexesBuffer;
        QVector3D min = QVector3D(__FLT_MAX__ ,__FLT_MAX__, __FLT_MAX__) ;
        QVector3D max = QVector3D(-__FLT_MAX__ ,-__FLT_MAX__,-__FLT_MAX__) ;

    public:

        AABB();

        AABB( std::vector<VertexData> &points );

        const QVector3D &getMin() const;
        void setMin(const QVector3D &newMin);

        const QVector3D &getMax() const;
        void setMax(const QVector3D &newMax);

        void resizeAABB( AABB &box );

        float getHeight();
        float getWidth();

        void initBuffers();
        void drawAABB( Shader * shader );

        void transformAABB( QMatrix4x4 model );
};




#endif // AABB_H
