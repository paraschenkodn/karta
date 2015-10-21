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
    delete spherepoints;
}

void Scene::initializeGL() {
    //
    qDebug() << QString((const char*)glGetString(GL_VERSION)) << "\n" << QString((const char*)glGetString(GL_VENDOR))<< "\n" << QString((const char*)glGetString(GL_RENDERER));//<< "\n" << glGetString(GL_EXTENTIONS);

    // очищаем поле
    glClearColor(0.1f,0.1f,0.2f,1.0f); //


    //создаём объект - треугольник (создаём один, а нарисовать можем много таких-же)
    //m_triangle=new Triangle(&m_program, m_vertexAttr, m_colorAttr, m_texAttr, m_texUniform);
    m_triangle=new Triangle();

    // инициализируем массив координат для сетевой сферы
    spherepoints=new pointsofsphere(300);

    // тест - создаём простую сферу на шейдерах
    m_shphere=new shphere();
}

void Scene::paintGL(){
    //setStates();
    //glClear(GL_COLOR_BUFFER_BIT);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //glMatrixMode(GL_PROJECTION);
    //qgluPerspective(60.0, width / height, 0.01, 15.0);

    //glMatrixMode(GL_MODELVIEW);

    //пишем матрицу ориентации
    QMatrix4x4 viewport;
    // устанавливаем трёхмерную канву (в перспективной проекции) для рисования (плоскости отсечения)
    // угол перспективы, отношение сторон, расстояние до ближней отсекающей плоскости и дальней
    ///viewport.perspective(60.0f,3.0f/4.0f,0.1f,400.0f);  // glFrustum( xmin, xmax, ymin, ymax, near, far)  // gluPerspective(fovy, aspect, near, far)
    // устанавливаем трёхмерную канву (в ортогональной проекции) для рисования (плоскости отсечения)
    viewport.ortho(-2.0f,2.0f,-2.0f,2.0f,3.0f,-3.0f); // glOrtho(left,right,bottom,top,near,far) // увеличение уменьшает фигуры на сцене (по Z задаём больше, чтобы не видеть отсечение фигур)
    // переносим по z дальше
    ///viewport.translate(0.0f,0.0f,-5.0f);
    // изменяем масштаб фигуры (увеличиваем)
    viewport.scale(2.0f);
    // указываем угол поворота и ось поворота смотрящую из центра координат к точке x,y,z,
    viewport.rotate(m_angle,0.0f,1.0f,0.0f);

    // РИСУЕМ ТРЕУГОЛЬНИК
    // инициализируем данные программы матрицы и атрибуты
    m_triangle->init();
    // зaпихиваем в его программу матрицу ориентации view
    m_triangle->m_program.setUniformValue(m_triangle->m_matrixUniform, viewport);
    // вызываем функцию рисования объекта (или объектов в for)
    m_triangle->draw();
    // проводим сброс инициализации параметров
    m_triangle->drop();//*/

    //РИСУЕМ СФЕРЫ
    m_shphere->init();
    m_shphere->m_program.setUniformValue(m_shphere->m_matrixUniform, viewport);
    m_shphere->draw();
    m_shphere->drop();//*/
}

void Scene::resizeGL(int w, int h){
  glViewport(0,0,w,h);
}

void Scene::setStates()
{
    // glEnable(GL_BRAINS) // ;)

    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_CULL_FACE); // активирует устранение спрятанных поверхностей.
    glEnable(GL_LIGHTING);
    glEnable(GL_COLOR_MATERIAL);  // было отключено
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_NORMALIZE);

    /*/ формирование пирамиды видимости:
    glMatrixMode(GL_PROJECTION);    // Матрица проекции
    glPushMatrix();                 //
    glLoadIdentity();               // задание единичной матрицы (установки текущей матрицы в единичную)
    // glFrustum( xmin, xmax, ymin, ymax, near, far) // функция задания перспективы
    // glViewport(0, 0, ClientWidth, ClientHeight); // Задание области вывода
    glMatrixMode(GL_MODELVIEW);     //Переключение на матрицу модели.
    glPushMatrix();
    glLoadIdentity();
    //InvalidateRect(Handle, nil, False);// Перерисовка окна
    //*/

    setLights();

    float materialSpecular[] = {0.5f, 0.5f, 0.5f, 1.0f};
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, materialSpecular);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 32.0f);
}

void Scene::setLights()
{
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    float lightColour[] = {1.0f, 0.9f, 0.9f, 1.0f};
    //float lightColour[] = {1.0f, 1.0f, 1.0f, 1.0f};
    //float lightDir[] = {0.0f, 0.0f, 1.0f, 0.0f};
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColour);
    //glLightfv(GL_LIGHT0, GL_SPECULAR, lightColour);
    //glLightfv(GL_LIGHT0, GL_POSITION, lightDir);
    glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, 1.0f);
    glEnable(GL_LIGHT0);
}

void Scene::defaultStates()
{
    //glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glDisable(GL_LIGHTING);
    //glDisable(GL_COLOR_MATERIAL);
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_LIGHT0);
    glDisable(GL_NORMALIZE);

    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, 0.0f);
    float defaultMaterialSpecular[] = {0.0f, 0.0f, 0.0f, 1.0f};
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, defaultMaterialSpecular);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0.0f);
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
