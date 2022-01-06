#ifndef OBJ_LOADER_H
#define OBJ_LOADER_H

#include <vector>
#include <iostream>
#include <fstream>
#include <QVector2D>
#include <QVector3D>
#include <QString>

#include "headers/render/vertexData.h"

using namespace std;

class ObjLoader{

private:
public:
    static void loadObject( string file, vector<VertexData> & vertexDatas, vector<int> &faces );
};

#endif // OBJ_LOADER_H
