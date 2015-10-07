#ifndef SCENE_H
#define SCENE_H

#include <QOpenGLWidget>
#include "triangle.h"
#include <QOpenGLShaderProgram>

class Scene : public QOpenGLWidget
{
public:
  Scene(QWidget *parent = 0);
  ~Scene();

private:
  void initializeGL();
  void paintGL();
  void resizeGL(int w, int h);

  Triangle *m_triangle;
  QOpenGLShaderProgram m_program;

  // создаём идентификаторы для обращения к шейдерным переменным
  int m_vertexAttr;
  int m_colorAttr;
  int m_matrixUniform;
};

#endif // SCENE_H
