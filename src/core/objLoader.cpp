#include "headers/core/objloader.h"

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
            int normalPointer = atoi( splited2[count    ].c_str() ) - 1;
            v.normal = normals[ normalPointer ];
        }

        vertexDatas[vertexPointer] = v;

    }
}

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

    while(  getline( input, line )  ){

        if ( line.rfind( "vn ", 0 ) == 0 ){
            hasNormal = true;
            vector<string> splited = split( line, " " );
            normals.push_back( QVector3D( atof( splited[1].c_str() ), atof( splited[2].c_str() ), atof( splited[3].c_str() ) ) );

        }else if ( line.rfind( "vt ", 0 ) == 0 ){
            hasTexcoord = true;
            vector<string> splited = split( line, " " );
            textureCoords.push_back( QVector2D( atof( splited[1].c_str() ), atof( splited[2].c_str() ) ) );
        }
        else if( line.rfind( "v ", 0 ) == 0 ){

                    vector<string> splited = split( line, " " );
                    positions.push_back( QVector3D( atof( splited[1].c_str() ), atof( splited[2].c_str() ), atof( splited[3].c_str() ) ) );

        }else if ( line.rfind( "f ", 0 ) == 0 ){
            break;
        }

        else{
            continue;
        }

    }

    vertexDatas.resize( positions.size() );
    faces.resize( positions.size() * 3 );

    do{

        if ( line.rfind( "f ", 0 ) == 0 )

            processVertex( line, vertexDatas, faces, positions, normals, textureCoords, hasNormal, hasTexcoord );

    }while( getline( input, line ) );

}



