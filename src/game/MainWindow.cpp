#include "headers/game/MainWindow.h"

MainWindow::MainWindow( QWidget * parent ) :
    QMainWindow(parent)
{
    this->resize( 1366, 768 );

}

MainWindow::MainWindow( QScopedPointer<CoreEngine> &coreEngine,  QWidget * parent ) :
    QMainWindow(parent)
{
    this->coreEngine.swap( coreEngine );
    this->setCentralWidget( this->coreEngine.data() );
}





