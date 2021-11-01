#ifndef AABB_H
#define AABB_H

#include<QVector3D>
#include<vector>

class AABB{

private:
    QVector3D min = QVector3D(__FLT_MAX__ ,__FLT_MAX__, __FLT_MAX__) ;
    QVector3D max = QVector3D(-__FLT_MAX__ ,-__FLT_MAX__,-__FLT_MAX__) ;


public:
    AABB(){}
    AABB( std::vector<QVector3D> points ){
       for( unsigned int i = 0; i < points.size(); i++){
            if (min.x() > points[i].x())
                min.setX( points[i].x() );

            if (min.y() > points[i].y())
                min.setY( points[i].y() );

            if (min.z() > points[i].z())
                min.setZ( points[i].z() );

            if (max.x() < points[i].x())
                max.setX( points[i].x() );

            if (max.x() > points[i].y())
                max.setY( points[i].y() );

            if (max.z() < points[i].z())
                max.setZ( points[i].z() );
       }
    }
};

#endif // AABB_H
