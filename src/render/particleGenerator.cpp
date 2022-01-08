#include "headers/render/particleGenerator.h"

using Random = effolkronium::random_static;

void ParticleGenerator::setCamera(GameObjectCamera *newCamera)
{
    camera = newCamera;
}

float ParticleGenerator::getRange() const
{
    return range;
}

void ParticleGenerator::setRange(float newRange)
{
    range = newRange;
}

ParticleGenerator::ParticleGenerator(){

}

ParticleGenerator::ParticleGenerator( float number, Texture &sprite, QVector3D particlesSpawnPoint, QVector3D particleDirection, bool renderPoint ){
    this->maxParticles = number;
    this->sprite = sprite;
    this->context = QOpenGLContext::currentContext();
    glFuncs = this->context->versionFunctions<QOpenGLFunctions_3_3_Core>();

    this->particleDirection = particleDirection;
    this->particlesSpawnPoint = particlesSpawnPoint;
    this->renderPoint = renderPoint;

    for (unsigned int i = 0; i < number; ++i){
        particles.push_back(Particle());
        positions.push_back(QVector4D());
        colors.push_back(QVector4D());
    }
    for (unsigned int i = 0; i < number; ++i){
        particles[i].life = -1.0f;
        particles[i].cameradistance = -1.0f;
    }

    initBuffers();
    initGeometry();
}

ParticleGenerator::ParticleGenerator( float number, QVector3D particlesSpawnPoint, QVector3D particleDirection, QVector3D color, bool renderPoint ){
    this->maxParticles = number;

    this->context = QOpenGLContext::currentContext();
    glFuncs = this->context->versionFunctions<QOpenGLFunctions_3_3_Core>();

    this->particleDirection = particleDirection;
    this->particlesSpawnPoint = particlesSpawnPoint;
    this->renderPoint = renderPoint;
    this->color = color;

    for (unsigned int i = 0; i < number; ++i){
        particles.push_back(Particle());
        positions.push_back(QVector4D());
        colors.push_back(QVector4D());
    }
    for (unsigned int i = 0; i < number; ++i){
        particles[i].life = -1.0f;
        particles[i].cameradistance = -1.0f;
    }

    initBuffers();
    initGeometry();
}


void ParticleGenerator::initBuffers(){

    this->vBuffer = QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    this->positionsBuffer = QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    this->colorsBuffer = QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);

    this->vBuffer.create();
    this->positionsBuffer.create();
    this->colorsBuffer.create();

}

void ParticleGenerator::initGeometry(){   

    if ( ! renderPoint ){
        particleData.push_back( { QVector3D( -1.0f,  1.0f, 0.0f ), QVector3D( 0.0f,  0.0f, 1.0f ), QVector2D( 0.0f, 1.0f ) });
        particleData.push_back( { QVector3D( -1.0f, -1.0f, 0.0f ), QVector3D( 0.0f,  0.0f, 1.0f ), QVector2D( 0.0f, 0.0f ) });
        particleData.push_back( { QVector3D(  1.0f,  1.0f, 0.0f ), QVector3D( 0.0f,  0.0f, 1.0f ), QVector2D( 1.0f, 1.0f ) });
        particleData.push_back( { QVector3D(  1.0f, -1.0f, 0.0f ), QVector3D( 0.0f,  0.0f, 1.0f ), QVector2D( 1.0f, 0.0f ) });
    }
    else {
        particleData.push_back( { QVector3D(  0.0f,  0.0f, 0.0f ), QVector3D( 0.0f,  0.0f, 1.0f ), QVector2D( 1.0f, 1.0f ) });
    }
    this->vBuffer.bind();
    this->vBuffer.allocate(particleData.data(), 4 * sizeof(VertexData) );

}

uint ParticleGenerator::firstUnusedParticle()
{
    for (uint i = lastUsedParticle; i < maxParticles; ++i) {
        if (particles[i].life <= 0.0f){
            lastUsedParticle = i;
            return i;
        }
    }

    for(uint i=0; i<lastUsedParticle; i++){
        if (particles[i].life < 0){
            lastUsedParticle = i;
            return i;
        }
    }

    // override first particle if all others are alive
    lastUsedParticle = 0;
    return 0;
}

void ParticleGenerator::sortParticles(){
    std::sort(&particles[0], &particles[maxParticles]);
}



void ParticleGenerator::update( float dt ){
    newParticles = (int)(dt*10000.0);

    for(int i=0; i<newParticles; i++){
            int particleIndex = firstUnusedParticle();
            particles[particleIndex].life = Random::get( 1, 30); // This particle will live 5 seconds.
            particles[particleIndex].position =     this->particlesSpawnPoint + QVector3D(
                                Random::get( -0., 0. ),
                                Random::get( 1., 3. ),
                                Random::get( -range, range )
                            );

            float spread = 1.1f;

            QVector3D randomdir =QVector3D(
                        Random::get( -0.1, 0.1 ),
                        Random::get( -0.1, 0.1 ),
                        Random::get( -0.1, 0.1 )
            );

            particles[particleIndex].speed =  particleDirection + randomdir * spread;

            particles[particleIndex].color = this->color;
            particles[particleIndex].color.setW( particles[particleIndex].life );

            particles[particleIndex].size = (rand()%1000)/2000.0f + 0.1f;

    }

    // Simulate all particles
    particlesCount = 0;
    for(int i=0; i<maxParticles; i++){

        Particle& p = particles[i]; // shortcut
        if(p.life > 0.0f){

            // Decrease life
            p.life -= dt;
            if (p.life > 0.0f){

                // Simulate simple physics : gravity only, no collisions
                p.speed += 3 *  particleDirection * dt * 0.2f;
                p.position +=  p.speed * dt;
                QVector3D view = p.position - camera->getCameraComponent()->getCameraPosition();
                p.cameradistance = view.lengthSquared();

                // Fill the GPU buffer
                positions[i] = QVector4D( p.position, p.size );
                colors[i] = p.color;

            }else{
                // Particles that just died will be put at the end of the buffer in SortParticles();
                p.cameradistance = -1.0f;
            }

            particlesCount++;

        }
    }

    sortParticles();

}

void ParticleGenerator::render( Shader * shader ){
    shader->useShaderProgram();
    QMatrix4x4 view = camera->getCameraComponent()->getViewMatrix();
    QVector3D rightW = QVector3D( view.column(0).x(), view.column(0).y(),view.column(0).z() );
    QVector3D up = QVector3D( view.column(0).y(), view.column(1).y(),view.column(2).y() );

    shader->setUniformValue( "cameraRight_W", rightW);
    shader->setUniformValue( "cameraUp_W", up );
    shader->setUniformValue( "model", QMatrix4x4());
    shader->setUniformValue( "view", view );
    shader->setUniformValue( "projection", camera->getCameraComponent()->getProjection() );

    quintptr offset = 0;

    if( !  renderPoint ){

        this->sprite.bindTexture( 0, shader );

        vBuffer.bind();

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

        positionsBuffer.bind();
        positionsBuffer.allocate( positions.data(), particlesCount * sizeof(QVector4D) );

        int xyzsLocation = shader->getProgram().attributeLocation("xyzs");
        shader->getProgram().enableAttributeArray(xyzsLocation);
        shader->getProgram().setAttributeBuffer(xyzsLocation, GL_FLOAT, 0 , 4, sizeof(QVector4D));

        colorsBuffer.bind();
        colorsBuffer.allocate( colors.data(), particlesCount * sizeof(QVector4D) );

        int colorLocation = shader->getProgram().attributeLocation("color");
        shader->getProgram().enableAttributeArray(colorLocation);
        shader->getProgram().setAttributeBuffer(colorLocation, GL_FLOAT, 0 , 4, sizeof(QVector4D));

        glFuncs->glVertexAttribDivisor(0, 0);
        glFuncs->glVertexAttribDivisor(1, 0);
        glFuncs->glVertexAttribDivisor(2, 0);
        glFuncs->glVertexAttribDivisor(3, 1);
        glFuncs->glVertexAttribDivisor(4, 1);

        glFuncs->glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, maxParticles);

        this->sprite.unbindTexture( 0 );

    }
    else{

        glFuncs->glEnable( GL_PROGRAM_POINT_SIZE );

        vBuffer.bind();

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

        positionsBuffer.bind();
        positionsBuffer.allocate( positions.data(), particlesCount * sizeof(QVector4D) );

        int xyzsLocation = shader->getProgram().attributeLocation("xyzs");
        shader->getProgram().enableAttributeArray(xyzsLocation);
        shader->getProgram().setAttributeBuffer(xyzsLocation, GL_FLOAT, 0 , 4, sizeof(QVector4D));

        colorsBuffer.bind();
        colorsBuffer.allocate( colors.data(), particlesCount * sizeof(QVector4D) );

        int colorLocation = shader->getProgram().attributeLocation("color");
        shader->getProgram().enableAttributeArray(colorLocation);
        shader->getProgram().setAttributeBuffer(colorLocation, GL_FLOAT, 0 , 4, sizeof(QVector4D));

        glFuncs->glVertexAttribDivisor(0, 0);
        glFuncs->glVertexAttribDivisor(1, 0);
        glFuncs->glVertexAttribDivisor(2, 0);
        glFuncs->glVertexAttribDivisor(3, 1);
        glFuncs->glVertexAttribDivisor(4, 1);

        glFuncs->glDrawArraysInstanced(GL_POINTS, 0, 4, maxParticles);
        glFuncs->glDisable( GL_PROGRAM_POINT_SIZE );

    }

}


