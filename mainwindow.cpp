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
  connect(ui->SceneWidget,SIGNAL(setPerspectiveInfo(QString)),ui->label,SLOT(setText(QString)));
  connect(ui->SceneWidget,SIGNAL(setFiguresInfo(QString)),ui->label_2,SLOT(setText(QString)));
}

MainWindow::~MainWindow()
{
    delete ui;
}
