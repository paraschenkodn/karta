#include "scene.h"

#include <QMatrix4x4>

const float step=0.01f; // шаг сдвига

Scene::Scene(QWidget *parent) :
    QOpenGLWidget (parent),
    m_angle(0)
{
  // задаём для виджета фокус, чтобы оно реагировало на кнопки
  this->setFocusPolicy(Qt::StrongFocus);
  // подключаем таймер к слоту
  connect(&m_timer,SIGNAL(timeout()),this,SLOT(slotAnimation()));
  // запускаем таймер
  m_timer.start(20);
}

Scene::~Scene()
{
delete m_triangle;
}

void Scene::initializeGL() {
    // очищаем поле
    glClearColor(0.1f,0.1f,0.2f,1.0f); //

    // инициализируем шейдеры
    QOpenGLShader vShader(QOpenGLShader::Vertex);
    vShader.compileSourceFile(":/Shaders/vShader.glsl");

    QOpenGLShader fShader(QOpenGLShader::Fragment);
    fShader.compileSourceFile(":/Shaders/fShader.glsl");

    //добавляем шейдеры в программу
    m_program.addShader(&vShader);
    m_program.addShader(&fShader);
    // линкуем программу и проверяем
    if (!m_program.link()){
        qWarning("Хъюстон, у нас проблемы:\nШейдерная программа не слинковалась");
        return; // Хъюстон, у нас проблемы
    }
    // устанавливаем привязку между приложением и шейдерами
    m_vertexAttr=m_program.attributeLocation("vertexAttr");
    m_colorAttr=m_program.attributeLocation("colorAttr");
    m_matrixUniform=m_program.uniformLocation("matrix");
    m_texAttr=m_program.attributeLocation("texAttr");
    m_texUniform=m_program.attributeLocation("texUniform");

    //создаём объект - треугольник
    m_triangle=new Triangle(&m_program, m_vertexAttr, m_colorAttr, m_texAttr, m_texUniform);
}

void Scene::paintGL(){
    glClear(GL_COLOR_BUFFER_BIT);
    //подключаем программу и проверяем
    if (!m_program.bind()){
        qWarning("Хъюстон, у нас проблемы:\nШейдерная программа не сбиндилась");
        return;
    }
    // убираем искажения текстур
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    //пишем матрицу ориентации
    QMatrix4x4 matrix;
    // устанавливаем трёхмерную канву (в перспективной проекции) для рисования (плоскости отсечения)
    // угол перспективы, отношение сторон, расстояние до ближней отсекающей плоскости и дальней
    matrix.perspective(60.0f,3.0f/4.0f,0.1f,400.0f);
    // устанавливаем трёхмерную канву (в ортогональной проекции) для рисования (плоскости отсечения)
    //matrix.ortho(-2.0f,2.0f,-2.0f,2.0f,2.0f,-2.0f);
    // переносим по z дальше
    matrix.translate(0.0f,0.0f,-5.0f);
    // изменяем масштаб фигуры (увеличиваем)
    matrix.scale(2.0f);
    // указываем угол поворота и ось поворота смотрящую из центра координат к точке x,y,z,
    matrix.rotate(m_angle,0.0f,1.0f,0.0f);
    // устанавливаем матрицу в шейдер
    m_program.setUniformValue(m_matrixUniform, matrix);

    // вызываем функцию рисования
    m_triangle->draw();


    // очищаем программу
    m_program.release();
}

void Scene::resizeGL(int w, int h){
  glViewport(0,0,w,h);
}

void Scene::keyPressEvent(QKeyEvent *event)
{
  switch (event->key()) {
    case Qt::Key_Up:
          m_triangle->sety0(m_triangle->m_y0+step);
      break;
    case Qt::Key_Left:
          m_triangle->setx0(m_triangle->m_x0-step);
      break;
    case Qt::Key_Right:
          m_triangle->setx0(m_triangle->m_x0+step);
      break;
    case Qt::Key_Down:
          m_triangle->sety0(m_triangle->m_y0-step);
      break;
  case Qt::Key_W:
    break;
  case Qt::Key_S:
    break;
  case Qt::Key_A:
        --m_angle;
        if (m_angle<0) m_angle=359;
    break;
  case Qt::Key_D:
        ++m_angle;
        if (m_angle>=360) m_angle=0;
    break;
    default:
      break;
    }
  update();
}

void Scene::slotAnimation()
{
  ///++m_angle; // поворот на 1 градус?
  ///if (m_angle>=360) m_angle=0;
  //m_triangle->setx0(m_triangle->m_x0-step);
  update();
}
