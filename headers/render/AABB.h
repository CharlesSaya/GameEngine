#ifndef AABB_H
#define AABB_H


#include<vector>

#include<QVector3D>
#include<QOpenGLBuffer>

#include "headers/render/ray.h"
#include "headers/render/shader.h"
#include "headers/render/vertexData.h"

class AABB {

    private:
        int indexCount;

        std::vector<QVector3D> vertices;
        std::vector<GLuint> lines ;

        QVector3D min = QVector3D(__FLT_MAX__ ,__FLT_MAX__, -__FLT_MAX__) ;
        QVector3D max = QVector3D(-__FLT_MAX__ ,-__FLT_MAX__,__FLT_MAX__) ;

        QVector3D lastMin = QVector3D(__FLT_MAX__ ,__FLT_MAX__, __FLT_MAX__) ;
        QVector3D lastMax = QVector3D(-__FLT_MAX__ ,-__FLT_MAX__,-__FLT_MAX__) ;

        QVector3D minDefault = QVector3D(__FLT_MAX__ ,__FLT_MAX__, __FLT_MAX__) ;
        QVector3D maxDefault = QVector3D(-__FLT_MAX__ ,-__FLT_MAX__,-__FLT_MAX__) ;
        QOpenGLBuffer AABBverticesBuffer, AABBindexesBuffer;

    public:

        AABB();

        AABB( std::vector<VertexData> &points );

        QVector3D &getMin();
        void setMin(const QVector3D &newMin);

        QVector3D &getMax();
        void setMax(const QVector3D &newMax);

        void resizeAABB( AABB& bBox  );
        void resetAABB();

        float getHeight();
        float getWidth();
        float getDepth();

        void initBuffers();
        void drawAABB( Shader * shader );

        void updateAABB( const  QMatrix4x4 &model );
        bool intersect(const Ray & ray);
        // Getters & setters

        int getIndexCount() const;
        void setIndexCount(int newIndexCount);

        const std::vector<QVector3D> &getVertices() const;
        void setVertices(const std::vector<QVector3D> &newVertices);

        const std::vector<GLuint> &getLines() const;
        void setLines(const std::vector<GLuint> &newLines);

        QVector3D &getMinDefault() ;
        void setMinDefault(const QVector3D &newMinDefault);
        QVector3D &getMaxDefault() ;
        void setMaxDefault(const QVector3D &newMaxDefault);

        const QVector3D &getLastMin() const;
        void setLastMin(const QVector3D &newLastMin);
        const QVector3D &getLastMax() const;
        void setLastMax(const QVector3D &newLastMax);
};




#endif // AABB_H
