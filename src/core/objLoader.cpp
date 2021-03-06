#include "headers/core/objloader.h"

/**
 * @brief Sépare une ligne représetant une face dans le fichier OBJ
 * @param line
 * @param delimiter
 * @return
 */

vector<string> split(  string line,  string delimiter ){

    size_t pos = 0;
    string token;
    vector<string> list;

    if( (pos = line.find(delimiter)) == string::npos )
        delimiter = "//";

    while ((pos = line.find(delimiter)) !=  string::npos) {
        token = line.substr(0, pos);
        list.push_back( token );
        line.erase(0, pos + delimiter.length());
    }
    list.push_back( line );

    return list;
}

/**
 * @brief Associe chaque triangle à ses verticex/normales/texcoord
 * @param line
 * @param vertexDatas
 * @param faces
 * @param positions
 * @param normals
 * @param textureCoords
 * @param hasNormal
 * @param hasTexcoord
 */

void processVertex( string line,
                    vector<VertexData> & vertexDatas,
                    vector<int> &faces,
                    vector<QVector3D> &positions,
                    vector<QVector3D> &normals,
                    vector<QVector2D> &textureCoords,
                    bool hasNormal, bool hasTexcoord ){

    vector<string> splited = split( line, " " );

    for( string s : vector<string>( splited.begin() + 1, splited.end() ) ){

        VertexData v;
        vector<string> splited2 = split( s, "/" );

        int vertexPointer  = atoi( splited2[0].c_str() ) - 1 ;
        faces.push_back( vertexPointer );

        v.position = positions[vertexPointer];
        int count = 1;
        if( hasTexcoord ){
            int texCoordPointer = atoi( splited2[count].c_str() ) - 1;
            v.texCoord = textureCoords[ texCoordPointer ];
            count++;
        }

        if( hasNormal ){
            int normalPointer = atoi( splited2[count].c_str() ) - 1;
            v.normal = normals[ normalPointer ];
        }
        vertexDatas[vertexPointer] = v;

    }
}

/**
 * @brief Charge l'objet afin de retourner les données parsées
 * @param file
 * @param vertexDatas
 * @param faces
 */

void ObjLoader::loadObject( string file, vector<VertexData> & vertexDatas, vector<int> &faces )
{

    vector<QVector3D> positions;
    vector<QVector3D> normals;
    vector<QVector2D> textureCoords;

    ifstream input;

    input.open(file);

    if ( !input ){

        return;
    }

    bool hasNormal = false, hasTexcoord = false;

    string line;
    int i = 0 ;
    while(  getline( input, line )  ){

        if ( line.rfind( "vn ", 0 ) == 0 ){
            hasNormal = true;
            vector<string> splited = split( line, " " );
            QString normal1= QString::fromStdString(splited[1].c_str());
            QString normal2 = QString::fromStdString(splited[2].c_str());
            QString normal3 = QString::fromStdString(splited[3].c_str());
            normals.push_back( QVector3D(normal1.toFloat(), normal2.toFloat() , normal2.toFloat()));

        }else if ( line.rfind( "vt ", 0 ) == 0 ){
            hasTexcoord = true;
            vector<string> splited = split( line, " " );
            QString texture1= QString::fromStdString(splited[1].c_str());
            QString texture2 = QString::fromStdString(splited[2].c_str() );
            textureCoords.push_back( QVector2D( texture1.toFloat(), texture2.toFloat() ) );
        }
        else if( line.rfind( "v ", 0 ) == 0 ){

            vector<string> splited = split( line, " " );
            QString vertex1= QString::fromStdString(splited[1].c_str());
            QString vertex2 = QString::fromStdString(splited[2].c_str());
            QString vertex3 = QString::fromStdString(splited[3].c_str());
            positions.push_back( QVector3D( vertex1.toFloat(), vertex2.toFloat(), vertex3.toFloat() ) );

        }else if ( line.rfind( "f ", 0 ) == 0 ){
            break;
        }

        else{
            continue;
        }
        i++;
    }

    vertexDatas.resize( positions.size() );

    do{

        if ( line.rfind( "f ", 0 ) == 0 )

            processVertex( line, vertexDatas, faces, positions, normals, textureCoords, hasNormal, hasTexcoord );

    }while( getline( input, line ) );


}



