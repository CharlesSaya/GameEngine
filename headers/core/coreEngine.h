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

#ifndef COREENGINE_H
#define COREENGINE_H

#include <math.h>

#include "headers/core/player.h"
#include "headers/core/cameraComponent.h"

#include "headers/render/light.h"
#include "headers/render/terrain.h"
#include "headers/render/renderingEngine.h"

#include "headers/game/game.h"
#include "headers/game/sceneGraph.h"

#include "headers/physics/physicsEngine.h"
#include "headers/physics/colliderEngine.h"

#include "transform.h"
#include "gameObject.h"
#include "meshrenderer.h"
#include "moveComponent.h"

#include <QTime>
#include <QBasicTimer>

#include <QtMath>
#include <QVector2D>
#include <QMatrix4x4>
#include <QQuaternion>
#include <QMouseEvent>

#include <QApplication>
#include <QOpenGLWidget>
#include <QOpenGLTexture>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions_3_3_Core>



class CoreEngine : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core
{
    Q_OBJECT

public slots:

signals:


public:
    explicit CoreEngine();
    explicit CoreEngine(int frames, QWidget *parent = 0);

protected:

    void initGame();
    void initSignals();
    void mousePressEvent( QMouseEvent *event) override;
    void mouseReleaseEvent( QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void mouseMoveEvent(QMouseEvent *e) override;
    void timerEvent(QTimerEvent *e) override;
    void keyPressEvent(QKeyEvent *key) override;
    void keyReleaseEvent( QKeyEvent * key) override;
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

private:

    int frames;

    float fixedStep   = 0.01f;
    float renderStep  = 0.0f;

    float deltaTime   = 0.0f;
    float lastFrame   = 0.0f;
    float accumulator = 0.0f;

    float yaw = 90., pitch  =0.;

    QTime time;
    QBasicTimer timer;

    QVector2D mousePressPosition;
    QVector3D rotationAxis;
    QQuaternion rotation;

    QMatrix4x4 projection;

    RenderingEngine renderingEngine;
    PhysicsEngine physicsEngine;
    ColliderEngine colliderEngine;

    Game * game;

    SceneGraph sceneGraph;
    Terrain terrain;
    CameraComponent * camera;

    QVector3D white = QVector3D( 1., 0., 0.);

    QSet<uint> inputs;

};

#endif // COREENGINE_H
