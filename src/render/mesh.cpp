#include "headers/render/mesh.h"

Mesh::Mesh( std::string filepath, std::vector<Texture> textures, Shader * shader, QVector3D meshColor, bool renderAABB ){
    this->meshColor = meshColor;
    this->textures = textures;
    this->shader = shader;

    this-> verticesBuffer =  QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    this-> indexesBuffer  =  QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
    this-> AABBverticesBuffer =  QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    this-> AABBindexesBuffer  =  QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);

    this->verticesBuffer.create();
    this->indexesBuffer.create();
    this->AABBverticesBuffer.create();
    this->AABBindexesBuffer.create();

    this->loadGeometry( filepath );

    this->bBox = AABB( this->meshesVertexDatas[0] );

    this->renderAABB = renderAABB;
    initializeOpenGLFunctions();

}

Mesh::Mesh( Terrain terrain, std::vector<Texture> textures, Shader * shader,  QVector3D meshColor, bool renderAABB  ){
    this->meshColor = meshColor;
    this->textures = textures;
    this->shader = shader;

    this-> verticesBuffer =  QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    this->indexesBuffer  =  QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
    this-> AABBverticesBuffer =  QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    this-> AABBindexesBuffer  =  QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);

    this-> verticesBuffer.create();
    this-> indexesBuffer.create();
    this->AABBverticesBuffer.create();
    this->AABBindexesBuffer.create();

    this->meshesVertexDatas.push_back( terrain.getPlaneVertices() );
    this->meshesFaces.push_back( terrain.getPlaneIndices() );

    this->bBox = AABB( this->meshesVertexDatas[0] );
    this->bBox.getMaxDefault().setY( terrain.getMaximumHeight() );
    this->bBox.getMinDefault().setY( terrain.getMinimumHeight() );
    this->bBox.setMax( this->bBox.getMaxDefault() );
    this->bBox.setMin( this->bBox.getMinDefault() );
    this->renderAABB = renderAABB;

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
        perror ("");
        exit(0);
    }
    return files;
}

void Mesh::updateAABB( const QMatrix4x4& model ) {
    this->bBox.updateAABB( model );
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

void Mesh::bindTextures( Shader * shader ){
    for ( uint u = 0; u < this->textures.size() ; u++ )
        this->textures[u].bindTexture( u,  shader );
    bindShadowTexture();
}

void Mesh::bindShadowTexture(){
    glActiveTexture( GL_TEXTURE0 + this->textures.size() );
    glBindTexture( GL_TEXTURE_2D, m_shadowMapTex );
    shader->setUniformValue( "shadowTexture", uint(this->textures.size()));
}

void Mesh::unbindTextures(){
    for ( Texture texture : this->textures )
        texture.unbindTexture();
}

void Mesh::drawAABB( Shader * shader ){

    this->bBox.initBuffers();

    indexCount = this->bBox.getIndexCount();

    AABBverticesBuffer.bind();
    AABBverticesBuffer.allocate( this->bBox.getVertices().data(), this->bBox.getVertices().size() * sizeof( QVector3D ) );

    AABBindexesBuffer.bind();
    AABBindexesBuffer.allocate ( this->bBox.getLines().data(), indexCount * sizeof( GLuint ) );

    quintptr offset = 0;

    int vertexLocation = shader->getProgram().attributeLocation("a_position");
    shader->getProgram().enableAttributeArray(vertexLocation);
    shader->getProgram().setAttributeBuffer(vertexLocation, GL_FLOAT, offset, 3, sizeof(QVector3D));

    glDrawElements(GL_LINES, indexCount, GL_UNSIGNED_INT, 0);
}

void Mesh::drawMesh( float distance, Shader * shader ){

    this->initBuffers( getLod(distance ));

    quintptr offset = 0;

    int vertexLocation = shader->getProgram().attributeLocation("a_position");
    shader->getProgram().enableAttributeArray(vertexLocation);
    shader->getProgram().setAttributeBuffer(vertexLocation, GL_FLOAT, offset, 3, sizeof(VertexData));

    offset += sizeof(QVector3D);

    int normalLocation = shader->getProgram().attributeLocation("a_normal");
    shader->getProgram().enableAttributeArray(normalLocation);
    shader->getProgram().setAttributeBuffer(normalLocation, GL_FLOAT, offset , 3, sizeof(VertexData));

    offset += sizeof(QVector3D);

    int texcoordLocation = shader->getProgram().attributeLocation("a_texcoord");
    shader->getProgram().enableAttributeArray(texcoordLocation);
    shader->getProgram().setAttributeBuffer(texcoordLocation, GL_FLOAT, offset , 2, sizeof(VertexData));

    glDrawElements(GL_TRIANGLES, this->indexCount, GL_UNSIGNED_INT, 0);

}

void Mesh::setShadowTexture(uint m_shadowMapTex){
    this->m_shadowMapTex = m_shadowMapTex;
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

bool Mesh::getRenderAABB() const
{
    return renderAABB;
}

Mesh::Mesh(){

}
