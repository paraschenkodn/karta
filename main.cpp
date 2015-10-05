#include "mainwindow.h"
//#include <QApplication>
#include <QGuiApplication>
#include <lenswindow.h>
#include <oglw.h>

int main(int argc, char *argv[])
{
  QGuiApplication a(argc, argv);
  //MainWindow w;
  //w.show();
    //создаём окно рисования
    //LensWindow window;
    OGLW window;
    //window.resize(640,480);
    window.show();
    window.setAnimating(true);

  return a.exec();
}
