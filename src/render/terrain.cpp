
#include <iostream>
#include <QVector2D>
#include <QVector3D>

#include "headers/render/terrain.h"


using namespace std;

Terrain::Terrain(){

}

//! [0]
Terrain::Terrain( Texture &texture )

{
    this->texture = texture;

    this->width = 32;
    this->height = 32;

    this->gridSquareSize = gridSize / float( width - 1) ;
    this->gridNumber = gridSize / gridSquareSize ;

    initGeometry();

}

Terrain::~Terrain()
{

}

void Terrain::initGeometry()
{

    for( int i =0; i<height; i++){
       for (int j = 0; j<width; j++ ){
           planeVertices.push_back( { QVector3D( (float(j)/(float(width)-1.0)  - 0.5 ) * gridSize, 0.0, - (float(i)/(float(height)-1.0)  - 0.5 ) * gridSize ),
                                      QVector3D( 0., 1., 0.),
                                      QVector2D( float(j)/float(width-1), float(i)/float(height-1))
                                    } );
       }
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

float Terrain::getHeight( QVector3D &position ){
    QImage &im = this->texture.getImage();

    QVector3D relativePosition = position - QVector3D( 0.f, 0.f, 0.f );

    int gridX =  int( floor( relativePosition.x() / gridSquareSize ) );
    int gridZ =  int( floor( -relativePosition.z() / gridSquareSize ) );

    if ( gridX < (0) || gridX > int( gridNumber -1 ) || gridZ < (0) || gridZ > int( gridNumber - 1 ))
        return 0.f;

    std::vector<int> ind = map[ gridZ * width + gridX ];

    QVector2D a = planeVertices[ind[0]].texCoord * (im.width()-1);
    QVector2D b = planeVertices[ind[1]].texCoord * (im.width()-1);
    QVector2D c = planeVertices[ind[5]].texCoord * (im.width()-1);
    QVector2D d = planeVertices[ind[2]].texCoord * (im.width()-1);

    float hA = im.pixelColor(a.x(), a.y()).blue() / 255.;
    float hB = im.pixelColor(b.x(), b.y()).blue() / 255.;
    float hC = im.pixelColor(c.x(), c.y()).blue() / 255.;
    float hD = im.pixelColor(d.x(), d.y()).blue() / 255.;

    float posX = fmod(relativePosition.x(), gridSquareSize) / gridSquareSize;
    float posZ = fmod(relativePosition.z(), gridSquareSize) / gridSquareSize;

    if ( posX < 1. - posZ ){

        return baricentricHeight( QVector3D( 0., hA, 0. ),  QVector3D( 1., hB, 0. ),  QVector3D( 0., hD, 1. ), QVector2D( posX, posZ ) );

    }else{
        return baricentricHeight( QVector3D( 1., hD, 0. ),  QVector3D( 1., hB, 1. ),  QVector3D( 0., hC, 1. ), QVector2D( posX, posZ ) );
    }

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
    float maxHeight = -__FLT_MAX__;
    QImage &im = this->texture.getImage();
    for( int i = 0; i < this->height; i++ ){
        for( int j = 0; j < this->width; j++){
            if( im.pixelColor( j, i ).blue() > maxHeight )
                maxHeight = im.pixelColor( j, i ).blue() ;
        }
    }
    return maxHeight/255.0;
}

float Terrain::getMinimumHeight(){
    float minHeight = __FLT_MAX__;
    QImage &im = this->texture.getImage();
    for( int i = 0; i < this->height; i++ ){
        for( int j = 0; j < this->width; j++){
            if( im.pixelColor( j, i ).blue() < minHeight )
                minHeight = im.pixelColor( j, i ).blue() ;
        }
    }
    return minHeight/255.0;
}

std::vector<GLuint> &Terrain::getPlaneIndices()
{
    return planeIndices;
}

void Terrain::setPlaneIndices(const std::vector<GLuint> &newPlaneIndices)
{
    planeIndices = newPlaneIndices;
}

 std::vector<VertexData> &Terrain::getPlaneVertices()
{
    return planeVertices;
}

void Terrain::setPlaneVertices(const std::vector<VertexData> &newPlaneVertices)
{
    planeVertices = newPlaneVertices;
}

