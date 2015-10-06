#include <QApplication>
#include <QGLFormat>

//#include "glwidget.h"
#include <QGuiApplication>
#include "oglw.h"

int main( int argc, char* argv[] )
{
 /*QApplication a( argc, argv );
 // Specify an OpenGL 3.3 format using the Core profile.
 // That is, no old-school fixed pipeline functionality
 QGLFormat glFormat;
 glFormat.setVersion( 3, 3 );
 glFormat.setProfile( QGLFormat::CoreProfile ); // Requires >=Qt-4.8.0
 glFormat.setSampleBuffers( true );
 // Create a GLWidget requesting our format
 GLWidget w( glFormat );
 w.show(); //*/

 QGuiApplication a(argc, argv);
 OGLW w;
 w.resize(640,480);
 w.show();
 w.setAnimating(true);

return a.exec();
}
