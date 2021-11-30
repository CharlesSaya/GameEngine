#include "headers/render/mesh.h"

Mesh::Mesh( std::string filepath, std::vector<Texture> textures, Shader * shader, QVector3D meshColor ){
    this->meshColor = meshColor;
    this->textures = textures;
    this->shader = shader;

    this-> verticesBuffer =  QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    this-> indexesBuffer  =  QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);

    this->verticesBuffer.create();
    this->indexesBuffer.create();

    this-> loadGeometry( filepath );

    this->bBox = AABB( this->meshesVertexDatas[0] );

    initializeOpenGLFunctions();

}

Mesh::Mesh( Terrain terrain, std::vector<Texture> textures, Shader * shader,  QVector3D meshColor ){
    this->meshColor = meshColor;
    this->textures = textures;
    this->shader = shader;

    this-> verticesBuffer =  QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    this->indexesBuffer  =  QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);

    this-> verticesBuffer.create();
    this-> indexesBuffer.create();

    this->meshesVertexDatas.push_back( terrain.getPlaneVertices() );
    this->meshesFaces.push_back( terrain.getPlaneIndices() );

    this->bBox = AABB( this->meshesVertexDatas[0] );

    initializeOpenGLFunctions();


}

std::vector<string> dirFiles( std::string dirName ){
    DIR *dir;
    struct dirent *ent;
    std::vector< string > files;
    if ((dir = opendir (dirName.c_str())) != NULL) {
        /* print all the files and directories within directory */
        while ((ent = readdir (dir)) != NULL) {
            if( !strcmp( ent->d_name, "." ) || !strcmp( ent->d_name, ".." ))
                continue;

            files.push_back( ent->d_name );
        }

        closedir (dir);

    } else {
        /* could not open directory */
        perror ("");
        exit(0);
    }
    return files;
}

void Mesh::updateBBox( Transform * transform ){
    this->bBox.transformAABB( transform->getModel() );
}

void Mesh::loadGeometry( std::string dirPath ){

    std::vector< string > files = dirFiles( dirPath );

    for( std::string filepath : files  ){

        std::vector<VertexData> lodVertices;
        std::vector<GLuint> lodFaces;

        std::vector<int> indices;

        ObjLoader::loadObject( dirPath + filepath, lodVertices, indices );
        this->meshesVertexDatas.push_back( lodVertices );

        for( int ind : indices )
            lodFaces.push_back( GLuint( ind ) );

        this->meshesFaces.push_back(lodFaces);

    }

}

uint Mesh::getLod( float distance ){

    if( distance > 5. && distance < 10.)
        return 1;
    else if( distance > 10.)
        return 2;

    return 0;
}

void Mesh::initBuffers( uint lod ){


    if ( lod >= this->meshesVertexDatas.size() )
        lod = this->meshesVertexDatas.size() - 1;

    a = this->meshesVertexDatas[lod];
    b = this->meshesFaces[lod];

    this->indexCount = b.size();
    this->verticesBuffer.bind();
    this->verticesBuffer.allocate( a.data(), a.size() * sizeof(VertexData));

    this->indexesBuffer.bind();
    this->indexesBuffer.allocate ( b.data(), this->indexCount  * sizeof(GLuint));

}

void Mesh::bindTextures(){
    for ( uint u = 0; u < this->textures.size() ; u++ )
        this->textures[u].bindTexture( u, this->shader );
}

void Mesh::unbindTextures(){
    for ( Texture texture : this->textures )
        texture.unbindTexture();
}


void Mesh::drawMesh( float distance ){
    this->initBuffers( getLod(distance ));

    this->verticesBuffer.bind();
    this->indexesBuffer.bind();

    quintptr offset = 0;

    int vertexLocation = this->shader->getProgram().attributeLocation("a_position");
    this->shader->getProgram().enableAttributeArray(vertexLocation);
    this->shader->getProgram().setAttributeBuffer(vertexLocation, GL_FLOAT, offset, 3, sizeof(VertexData));

    offset += sizeof(QVector3D);

    int normalLocation = this->shader->getProgram().attributeLocation("a_normal");
    this->shader->getProgram().enableAttributeArray(normalLocation);
    this->shader->getProgram().setAttributeBuffer(normalLocation, GL_FLOAT, offset , 3, sizeof(VertexData));

    offset += sizeof(QVector3D);

    int texcoordLocation = this->shader->getProgram().attributeLocation("a_texcoord");
    this->shader->getProgram().enableAttributeArray(texcoordLocation);
    this->shader->getProgram().setAttributeBuffer(texcoordLocation, GL_FLOAT, offset , 2, sizeof(VertexData));

    glDrawElements(GL_TRIANGLES, this->indexCount, GL_UNSIGNED_INT, 0);

}

const QVector3D &Mesh::getMeshColor() const
{

    return meshColor;
}

void Mesh::setMeshColor(const QVector3D &newMeshColor)
{
    meshColor = newMeshColor;
}


Shader *Mesh::getShader() const
{
    return shader;
}

void Mesh::setShader(Shader *newShader)
{
    shader = newShader;
}

const std::vector<Texture> &Mesh::getTextures() const
{
    return textures;
}

void Mesh::setTextures(const std::vector<Texture> &newTextures)
{
    textures = newTextures;
}

AABB &Mesh::getAABB()
{
    return bBox;
}

Mesh::Mesh(){

}
