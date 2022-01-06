
#include <iostream>
#include <QVector2D>
#include <QVector3D>

#include "headers/render/terrain.h"


using namespace std;

Terrain::Terrain(){

}

//! [0]
Terrain::Terrain( float gridSize, float scale, std::string file, Texture & heightmap )

{
    this->heightmap = heightmap;
    this->gridSize = gridSize;
    this->scale  = scale;

    this->width = heightmap.getImage().width();
    this->height = heightmap.getImage().height();

    this->terrainOBJ = file;

    this->gridSquareSize = gridSize / float( width - 1) ;
    this->gridNumber     = gridSize / gridSquareSize ;

    initGeometry();
}


void Terrain::initGeometry()
{

    for( int i =0; i<height; i++){
       for (int j = 0; j<width; j++ ){
           planeVertices.push_back( { QVector3D( (float(j)/(float(width)-1.0) ) * gridSize, 0.0, - (float(i)/(float(height)-1.0) ) * gridSize ),
                                      QVector3D( 0., 1., 0.),
                                      QVector2D( float(j)/float(width-1), float(i)/float(height-1))
                                    } );
       }
   }

    QImage &im = this->heightmap.getImage();

    for( uint i =0; i<planeVertices.size(); i++){
       float height = 0.0;
       VertexData v = planeVertices[i];
       height = im.pixelColor( v.texCoord.x() * (im.width()-1), v.texCoord.y() * (im.width()-1 )).blue()/255.;
       heights.push_back( height );
       planeVertices[i].position.setY( height * 30.156 );
   }

    for ( int i =0; i < height - 1; i++ ){
        for (int j = 0 ; j< width - 1; j++){
            std::vector<int> ind;

            planeIndices.push_back( GLuint(j + i * width) );
            ind.push_back( GLuint(j + i * width) );
            planeIndices.push_back( GLuint(j + 1  + i * width ) );
            ind.push_back( GLuint(j + 1 + i * width ) );
            planeIndices.push_back( GLuint(j + ( i + 1 ) * width ) );
            ind.push_back( GLuint(j + ( i + 1 ) * width ) );

            planeIndices.push_back( GLuint(j + ( i + 1 ) * width ) );
            ind.push_back( GLuint(j + ( i + 1 ) * width ) );
            planeIndices.push_back( GLuint(j + 1 + i * width ) );
            ind.push_back( GLuint(j + 1 + i * width ) );
            planeIndices.push_back( GLuint(j + 1 + ( i + 1 ) * width ) );
            ind.push_back( GLuint(j + 1 + ( i + 1 ) * width ) );

            map.insert(std::pair<int, std::vector<int>>( i*width + j, ind ));

        }
    }

}

const std::string &Terrain::getOBJFilename() const
{
    return terrainOBJ;
}


float Terrain::getScale() const
{
    return scale;
}

float Terrain::getHeightOfTerrain  ( QVector3D &position ){
    QImage &im = this->heightmap.getImage();

    QVector3D relativePosition = position - QVector3D();

    int gridX =  int( floor( relativePosition.x() / gridSquareSize ) );
    int gridZ =  int( floor( -relativePosition.z() / gridSquareSize ) );
//    qDebug() << gridX << gridZ;
    if ( (gridX < 0) || (gridX > im.width() -1)  || (gridZ < 0) || (gridZ > im.height() - 1 ) )
        return 0.f;


    float posX = fmod(relativePosition.x(), gridSquareSize) / gridSquareSize;
    float posZ = fmod(relativePosition.z(), gridSquareSize) / gridSquareSize;

    if ( posX < 1. - posZ )
        return baricentricHeight( QVector3D( 0., heights[ gridZ * im.width() + gridX ], 0. ),
                                  QVector3D( 1., heights[ ( gridZ + 1 ) * im.width() + gridX ], 0. ),
                                  QVector3D( 0., heights[ gridZ * im.width() + ( gridX + 1 ) ], 1. ),
                                  QVector2D( posX, posZ ) ) * maximumHeight;
    else
        return baricentricHeight( QVector3D( 0., heights[ ( gridZ + 1 ) * im.width() ], 0. ),
                                  QVector3D( 1., heights[ ( gridZ + 1 ) * im.width() + ( gridX + 1 ) ], 0. ),
                                  QVector3D( 0., heights[ gridZ * im.width() + ( gridX + 1 ) ], 1. ),
                                  QVector2D( posX, posZ ) ) * maximumHeight;
}

float Terrain::baricentricHeight(  QVector3D v0, QVector3D v1, QVector3D v2, QVector2D pos ){

    float det = ( v1.z() - v2.z() ) * ( v0.x() - v2.x() ) + ( v2.x() - v1.x() ) * (v0.z() - v2.z());
    float l1 = ( (v1.z() - v2.z()) * (pos.x() - v2.x()) + (v2.x() - v1.x()) * (pos.y() - v2.z()) ) / det;
    float l2 = ( (v2.z() - v0.z()) * (pos.x() - v2.x()) + (v0.x() - v2.x()) * (pos.y() - v2.z()) ) / det;
    float l3 = 1.0f - l1 - l2;

    return ( l1 * v0.y() )
         + ( l2 * v1.y() )
         + ( l3 * v2.y() );
}

float Terrain::getMaximumHeight(){
    return maximumHeight;
}
void Terrain::setMaximumHeight( float height ){
    maximumHeight = height;
}
float Terrain::getMinimumHeight(){
    float minHeight = __FLT_MAX__;
    QImage &im = this->heightmap.getImage();
    for( int i = 0; i < this->height; i++ ){
        for( int j = 0; j < this->width; j++){
            if( im.pixelColor( j, i ).blue() < minHeight )
                minHeight = im.pixelColor( j, i ).blue() ;
        }
    }
    return minHeight/255.0  * scale ;
}

std::vector<GLuint> &Terrain::getPlaneIndices()
{

    return planeIndices;
}

void Terrain::setPlaneIndices(const std::vector<GLuint> &newPlaneIndices)
{
    planeIndices = newPlaneIndices;

//    for ( int i =0; i < height - 1; i++ ){
//        for (int j = 0 ; j< width - 1; j++){

//            std::vector<int> ind;

//            ind.push_back( GLuint(j + i * width) );
//            ind.push_back( GLuint(j + 1 + i * width ) );
//            ind.push_back( GLuint(j + ( i + 1 ) * width ) );
//            ind.push_back( GLuint(j + ( i + 1 ) * width ) );
//            ind.push_back( GLuint(j + 1 + i * width ) );
//            ind.push_back( GLuint(j + 1 + ( i + 1 ) * width ) );

//            map.insert(std::pair<int, std::vector<int>>( i*width + j, ind ));

//        }
//    }

}

 std::vector<VertexData> &Terrain::getPlaneVertices()
{
    return planeVertices;
}

void Terrain::setPlaneVertices(const std::vector<VertexData> &newPlaneVertices)
{
    planeVertices = newPlaneVertices;
}

