#ifndef SCENE_H
#define SCENE_H

#include <QOpenGLWidget>
#include "triangle.h"
#include "pointsofsphere.h"
#include "shphere.h"
#include <QOpenGLShaderProgram>
#include <QKeyEvent>
#include <QTimer>
#include <QOpenGLFunctions>
#include <QOpenGLFunctions_3_0>

class Scene : public QOpenGLWidget
{
  Q_OBJECT
public:
  Scene(QWidget *parent = 0);
  ~Scene();

  QVector4D viewport;

private:
  void initializeGL();
  void paintGL();
  void resizeGL(int w, int h);

  // определяем метод изменяющий координаты положения треугольника
  void keyPressEvent(QKeyEvent *event);
  void mousePressEvent(QMouseEvent *event);      // работка с мышью, переопределяем функции обработки сообщений мыши (нажатие кнопок)
  void mouseReleaseEvent(QMouseEvent *event);    // переопределяем функции обработки сообщений мыши (отпускание кнопки)
  ///*********
  void mouseMoveEvent(QMouseEvent *event);       // переопределяем функции обработки сообщений мыши (перемещение)
  ///* если будем водить мышью по форме диалога с нажатой клавишей, сообщения также получает сцена
  /// ********
  void wheelEvent(QWheelEvent * event);  // переопределяем функции обработки сообщений мыши (колесо)

  // переменные камеры
  QVector3D cameraEye;
  QVector3D cameraCenter;
  QVector3D cameraUp;
  //int camEyePos; // позиция камеры () // + control of board??
  // енд камера

  Triangle *m_triangle;
  QOpenGLShaderProgram m_program;
  //QVector<QOpenGLShaderProgram *> m_program;
  shphere *m_shphere;

  pointsofsphere *spherepoints;

  // создаём идентификаторы для обращения к шейдерным переменным
  int m_vertexAttr;
  int m_colorAttr;
  int m_matrixUniform;
  int m_texAttr;
  int m_texUniform;

  QTimer m_timer;
  int m_angle; // текущий угол поворота

  void setLights();
  void setStates();
  void defaultStates();

  bool perspective;  // признак рисования в перспективной или в ортогональной проекции
public slots:
  // устанавливаем признак рисования в ортогональной проекции
  void setOrthogonal();
  // устанавливаем признак рисования в перспективной проекции
  void setPerspective();
private slots:
  // заставляем прорисовываться по таймеру
  void slotAnimation();
};

#endif // SCENE_H
