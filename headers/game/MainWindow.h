#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QScopedPointer>

#include "headers/core/coreEngine.h"

class MainWindow : public QMainWindow {

public:
    MainWindow( QWidget * parent = nullptr );
//    MainWindow( Game &game );
    MainWindow( QScopedPointer<CoreEngine> &coreEngine, QWidget * parent = nullptr );

    void initWindow();

private:
//    Game game;
    QScopedPointer<CoreEngine> coreEngine;


};

#endif // MAINWINDOW_H
