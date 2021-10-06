/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtCore module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "mainwidget.h"

#include <QMouseEvent>
#include <math.h>

MainWidget::MainWidget(int frames, QWidget *parent) :
    QOpenGLWidget(parent),
    geometries(0),
    rockTexture(0),
    angularSpeed(0),
    frames(frames)
{
    this->setWindowTitle( QString( QString::number( frames )) + "FPS" );
}

MainWidget::~MainWidget()
{
    // Make sure the context is current when deleting the texture
    // and the buffers.
    makeCurrent();
    delete rockTexture;
    delete geometries;
    doneCurrent();
}

//! [0]
void MainWidget::mousePressEvent(QMouseEvent *e)
{
    if (orbitalMode)
        return;
    // Save mouse press position
    mousePressPosition = QVector2D(e->localPos());
}

void MainWidget::mouseReleaseEvent(QMouseEvent *e)
{
    if (orbitalMode)
        return;

    // Mouse release position - mouse press position
    QVector2D diff = QVector2D(e->localPos()) - mousePressPosition;

    // Rotation axis is perpendicular to the mouse position difference
    // vector
    QVector3D n = QVector3D(diff.y(), diff.x(), 0.0).normalized();

    // Accelerate angular speed relative to the length of the mouse sweep
    qreal acc = diff.length() / 100.0;

    // Calculate new rotation axis as weighted sum
    rotationAxis = (rotationAxis * angularSpeed + n * acc).normalized();

    // Increase angular speed
    angularSpeed += acc;
}
//! [0]

//! [1]
void MainWidget::timerEvent(QTimerEvent *)
{
    // Decrease angular speed (friction)
    angularSpeed *= 0.8;

    if( orbitalMode) {

        orbitalCamera.cameraPosition = QVector3D( 20.* sin( time.elapsed() / 1000. ), orbitalCamera.cameraPosition.y(),
                                                   -20. - 20. *( - cos(time.elapsed() /  1000. )));
        update();
    }
    else {
        if( rotationMode ){
            rotation = QQuaternion::fromAxisAndAngle(QVector3D(0.,1.,0.), -rotationSpeed*( time.elapsed() / 1000. ));
            update();
        }
        else{
            if (angularSpeed < 0.01) {
                angularSpeed = 0.0;
            } else {
                rotation = QQuaternion::fromAxisAndAngle(rotationAxis, angularSpeed) * rotation;
                update();
            }
        }
    }
}
//! [1]

void MainWidget::initializeGL()
{
    initializeOpenGLFunctions();    //  essentiel pour lancer OpenGL

    glClearColor(0, 0, 0, 1);

    initShaders();
    initTextures();

//! [2]
    // Enable depth buffer
    glEnable(GL_DEPTH_TEST);

//    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE);
    // Enable back face culling
    glEnable(GL_CULL_FACE);
//! [2]

    geometries = new GeometryEngine;

    QVector3D cameraPosition = QVector3D(.0,10.0,-0.);
    QVector3D orbitalCameraPosition = QVector3D(.0,15.0,-0.);

    //Object center
    QVector3D cameraTarget = QVector3D(0.0,0.,-20.0);

    camera = Camera( cameraPosition, cameraTarget );
    orbitalCamera = Camera( orbitalCameraPosition, cameraTarget, true );

    time.start();
    // Use QBasicTimer because its faster than QTimer
    timer.start(1000/frames , this);
}

//! [3]
void MainWidget::initShaders()
{
    // Compile vertex shader
    if (!program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/vshader.glsl"))
        close();

    // Compile fragment shader
    if (!program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/fshader.glsl"))
        close();

    // Link shader pipeline
    if (!program.link())
        close();

    // Bind shader pipeline for use
    if (!program.bind())
        close();
}
//! [3]

//! [4]
void MainWidget::initTextures()
{
    // Load cube.png image
    rockTexture = new QOpenGLTexture(QImage(":/rock.png").mirrored());

    // Set nearest filtering mode for texture minification
    rockTexture->setMinificationFilter(QOpenGLTexture::Nearest);

    // Set bilinear filtering mode for texture magnification
    rockTexture->setMagnificationFilter(QOpenGLTexture::Linear);

    // Wrap texture coordinates by repeating
    // f.ex. texture coordinate (1.1, 1.2) is same as (0.1, 0.2)
    rockTexture->setWrapMode(QOpenGLTexture::Repeat);

    // Load cube.png image
    grassTexture = new QOpenGLTexture(QImage(":/grass.png").mirrored());

    // Set nearest filtering mode for texture minification
    grassTexture->setMinificationFilter(QOpenGLTexture::Nearest);

    // Set bilinear filtering mode for texture magnification
    grassTexture->setMagnificationFilter(QOpenGLTexture::Linear);

    // Wrap texture coordinates by repeating
    // f.ex. texture coordinate (1.1, 1.2) is same as (0.1, 0.2)
    grassTexture->setWrapMode(QOpenGLTexture::Repeat);

    // Load cube.png image
    snowTexture = new QOpenGLTexture(QImage(":/snowrocks.png").mirrored());

    // Set nearest filtering mode for texture minification
    snowTexture->setMinificationFilter(QOpenGLTexture::Nearest);

    // Set bilinear filtering mode for texture magnification
    snowTexture->setMagnificationFilter(QOpenGLTexture::Linear);

    // Wrap texture coordinates by repeating
    // f.ex. texture coordinate (1.1, 1.2) is same as (0.1, 0.2)
    snowTexture->setWrapMode(QOpenGLTexture::Repeat);

    heightMap = new QOpenGLTexture( QImage(":/heightmap-1024x1024.png") );

    // Set nearest filtering mode for texture minification
    heightMap->setMinificationFilter(QOpenGLTexture::Nearest);

    // Set bilinear filtering mode for texture magnification
    heightMap->setMagnificationFilter(QOpenGLTexture::Linear);

    // Wrap texture coordinates by repeating
    // f.ex. texture coordinate (1.1, 1.2) is same as (0.1, 0.2)
    heightMap->setWrapMode(QOpenGLTexture::Repeat);
}
//! [4]

//! [5]
void MainWidget::resizeGL(int w, int h)
{
    // Calculate aspect ratio
    qreal aspect = qreal(w) / qreal(h ? h : 1);

    // Set near plane to 3.0, far plane to 7.0, field of view 45 degrees
    const qreal zNear = 5.0, zFar = 100.0, fov = 80.0;

    // Reset projection
    projection.setToIdentity();

    // Set perspective projection
    projection.perspective(fov, aspect, zNear, zFar);
}
//! [5]

void MainWidget::keyPressEvent(QKeyEvent *key){
    float cameraSpeed =.2;

    switch( key->key() ){
        case Qt::Key_Up:
            if (rotationMode)
                rotationSpeed+=20.;
            else
                camera.cameraPosition += cameraSpeed * cameraFront;
            break;
        case Qt::Key_Down:
            if (rotationMode)
                if (rotationSpeed - 20. < 0.)
                    rotationSpeed = 0.;
                else
                    rotationSpeed-=20.;
            else
                camera.cameraPosition -= cameraSpeed * cameraFront;
            break;
        case Qt::Key_Right:
            if (rotationMode)
                return;
            else
                camera.cameraPosition += cameraSpeed * QVector3D::crossProduct(cameraFront,up).normalized() ;
            break;
        case Qt::Key_Left:
            if (rotationMode)
                return;
            else
                camera.cameraPosition -= cameraSpeed * QVector3D::crossProduct(cameraFront,up).normalized() ;
            break;
        case Qt::Key_C:
            time.restart();
            if( orbitalMode ){
                orbitalMode = false;
            }
            else{
                orbitalMode = true;
            }
            break;
        case Qt::Key_R:
            time.restart();
            if (!orbitalMode){
                if( rotationMode )
                    rotationMode = false;

                else{
                    rotationSpeed = 5.;
                    rotationMode = true;
                }
            }
            break;

    }
    update();
}

void MainWidget::paintGL()
{
    // Clear color and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    rockTexture->bind(0);
    grassTexture->bind(1);
    snowTexture->bind(2);
    heightMap->bind(3);

    //! [6]
    // Calculate model view transformation
    QMatrix4x4 model;
    model.translate(0.0,.0,-20.0);
    model.rotate(rotation);
    model.rotate(QQuaternion::fromAxisAndAngle(1.,0.,0.,90));

    if (orbitalMode){
        view = orbitalCamera.getViewMatrix();
    }
    else
        view = camera.getViewMatrix() ;

    program.setUniformValue("mvp_matrix", projection * view * model);
    //! [6]

    program.setUniformValue("rockTexture", 0);
    program.setUniformValue("grassTexture", 1);
    program.setUniformValue("snowTexture", 2);
    program.setUniformValue("heightMap",3);

    // Draw cube geometry
    geometries->drawCubeGeometry(&program);

    rockTexture->release();
    grassTexture->release();
    snowTexture->release();
    heightMap->release();

}
