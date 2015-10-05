#ifndef GRAPHWIDGET_H
#define GRAPHWIDGET_H

#include <QGraphicsView>
#include <QGroupBox>
#include <QMouseEvent>

//![0]
class GraphWidget : public QGraphicsView
{
  Q_OBJECT
public:
  GraphWidget(QWidget *parent = 0);
  void itemMoved(); // перемещение нода

public slots:
    void zoomIn();
    void zoomOut();

protected:
    void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;  // перехват сообщений клавиатуры
    void timerEvent(QTimerEvent *event) Q_DECL_OVERRIDE;  // перехват таймера
#ifndef QT_NO_WHEELEVENT
    void wheelEvent(QWheelEvent *event) Q_DECL_OVERRIDE; // перехват колеса мыши
#endif
    //void drawBackground(QPainter *painter, const QRectF &rect) Q_DECL_OVERRIDE;  //перехват прорисовки полотна

    void scaleView(qreal scaleFactor); // масштабирование просмотра

private:
    int timerId; // обновление состояния по таймеру
    //Node *selectedNode; // активная (выбранная) нода ??
};
//! [0]

class GroupBox:public QGroupBox
{
  Q_OBJECT
public:
  GroupBox ( const QString & title, QWidget * parent = 0 ):
    QGroupBox(title,parent)
  {
  setCheckable (false);
  }

    void mousePressEvent(QMouseEvent *event)
    {
        // Запоминаем позицию при нажатии кнопки мыши
        mpos = event->pos();
    }

    void mouseMoveEvent(QMouseEvent *event)
      {
      if (mpos.x() >= 0 && event->buttons() && Qt::LeftButton)
        {
            QPoint diff = event->pos() - mpos;
            QPoint newpos = this->pos() + diff;
            this->move(newpos);
        }
    }

    void mouseReleaseEvent(QMouseEvent *)
    {
        // Очищаем старое значение позиции нажатия мыши
        mpos = QPoint(-1, -1);
    }//*/

private:
    QPoint mpos;
};


#endif // GRAPHWIDGET_H
