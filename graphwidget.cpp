#include "graphwidget.h"

#include <QKeyEvent>
//#include <QPushButton>
#include <QGraphicsProxyWidget>
#include <QGraphicsRectItem>
#include <QtGui>
//#include <QLabel>
//#include <QLineEdit>
#include <QFormLayout>
#include <QEvent>
//#include <QPainter>

QGroupBox *groupBox;
QGroupBox *controlBox;

GraphWidget::GraphWidget(QWidget *parent)
  : QGraphicsView(parent), timerId(0)
{
  QGraphicsScene *scene = new QGraphicsScene(this);
  //scene->setItemIndexMethod(QGraphicsScene::NoIndex);
  //scene->setSceneRect(-200, -200, 400, 400);
  setScene(scene);
  setCacheMode(CacheBackground);
  setViewportUpdateMode(BoundingRectViewportUpdate);
  setRenderHint(QPainter::Antialiasing);
  setTransformationAnchor(AnchorUnderMouse);
  scale(qreal(0.5), qreal(0.5));
  setMinimumSize(400, 400);
  setWindowTitle(tr("Elastic Nodes"));
  setDragMode(QGraphicsView::ScrollHandDrag); // manual release dragmode http://www.cyberforum.ru/qt/thread564328.html

}

