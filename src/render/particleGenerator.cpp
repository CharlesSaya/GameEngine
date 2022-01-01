#include "headers/render/particleGenerator.h"

ParticleGenerator::ParticleGenerator(){

}

ParticleGenerator::ParticleGenerator( float number, Texture &sprite, GameObjectCamera * camera  ){
    this->number = number;
    this->sprite = sprite;
    this->camera = camera;
    this->context = QOpenGLContext::currentContext();
    glFuncs = this->context->versionFunctions<QOpenGLFunctions_3_3_Core>();

    for (unsigned int i = 0; i < number; ++i){
        particles.push_back(Particle());
        positions.push_back(QVector4D());
        colors.push_back(QVector4D());
    }
    for (unsigned int i = 0; i < number; ++i){
        particles[i].life = -1.0f;
        particles[i].cameradistance = -1.0f;
    }

    newParticles = (int)(0.016f*10000.0);
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

    particleData.push_back( { QVector3D( -1.0f,  1.0f, 0.0f ), QVector3D( 0.0f,  0.0f, 1.0f ), QVector2D( 0.0f, 1.0f ) });
    particleData.push_back( { QVector3D( -1.0f, -1.0f, 0.0f ), QVector3D( 0.0f,  0.0f, 1.0f ), QVector2D( 0.0f, 0.0f ) });
    particleData.push_back( { QVector3D(  1.0f,  1.0f, 0.0f ), QVector3D( 0.0f,  0.0f, 1.0f ), QVector2D( 1.0f, 1.0f ) });
    particleData.push_back( { QVector3D(  1.0f, -1.0f, 0.0f ), QVector3D( 0.0f,  0.0f, 1.0f ), QVector2D( 1.0f, 0.0f ) });

    this->vBuffer.bind();
    this->vBuffer.allocate(particleData.data(), 4 * sizeof(VertexData) );

}

uint ParticleGenerator::firstUnusedParticle()
{
    for (uint i = lastUsedParticle; i < number; ++i) {
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
    std::sort(&particles[0], &particles[number]);
}

void ParticleGenerator::update( float dt ){
    newParticles = (int)(dt*10000.0);

    for(int i=0; i<newParticles; i++){
            int particleIndex = firstUnusedParticle();
            particles[particleIndex].life = 5.0f; // This particle will live 5 seconds.
            particles[particleIndex].position = QVector3D(0.,0.,0.0f);

            float spread = 1.5f;
            QVector3D maindir = QVector3D(0.0f, -10.0f, 0.0f);

            QVector3D randomdir =QVector3D(
                (rand()%2000 - 1000.0f)/1000.0f,
                (rand()%2000 - 1000.0f)/1000.0f,
                (rand()%2000 - 1000.0f)/1000.0f
            );

            particles[particleIndex].speed = maindir + randomdir*spread;

            particles[particleIndex].color = QVector4D( 1.0, 1.0, 1.0, 1.0 );
            particles[particleIndex].color.setW( rand() % 256 );

            particles[particleIndex].size = (rand()%1000)/2000.0f + 0.1f;

        }

    // Simulate all particles
    particlesCount = 0;
    for(int i=0; i<number; i++){

        Particle& p = particles[i]; // shortcut
        if(p.life > 0.0f){

            // Decrease life
            p.life -= dt;
            if (p.life > 0.0f){

                // Simulate simple physics : gravity only, no collisions
                p.speed +=  QVector3D(0.0f,-9.81f, 0.0f) * (float)dt * 0.5f;
                QVector3D view = p.position - camera->getCameraComponent()->getCameraPosition();
                p.cameradistance = view.lengthSquared();
                //ParticlesContainer[i].pos += glm::vec3(0.0f,10.0f, 0.0f) * (float)delta;

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
    shader->setUniformValue( "view", view );
    shader->setUniformValue( "projection", camera->getCameraComponent()->getProjection() );

    this->sprite.bindTexture( 0, shader );
    vBuffer.bind();

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

    positionsBuffer.bind();
    positionsBuffer.allocate( positions.data(), particlesCount * sizeof(QVector4D) );

    int xyzsLocation = shader->getProgram().attributeLocation("xyzs");
    shader->getProgram().enableAttributeArray(xyzsLocation);
    shader->getProgram().setAttributeBuffer(xyzsLocation, GL_FLOAT, 0 , 2, sizeof(QVector4D));

    colorsBuffer.bind();
    colorsBuffer.allocate( colors.data(), particlesCount * sizeof(QVector4D) );

    int colorLocation = shader->getProgram().attributeLocation("color");
    shader->getProgram().enableAttributeArray(colorLocation);
    shader->getProgram().setAttributeBuffer(colorLocation, GL_FLOAT, 0 , 2, sizeof(QVector4D));

    glFuncs->glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, number);

}


