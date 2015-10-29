#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "scene.h"

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  connect(ui->action,SIGNAL(changed()),ui->SceneWidget,SLOT(setOrthogonal()));
  connect(ui->action_2,SIGNAL(changed()),ui->SceneWidget,SLOT(setPerspective()));
}

MainWindow::~MainWindow()
{
  delete ui;
}
