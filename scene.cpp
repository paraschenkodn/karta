#include "scene.h"

#include <QMatrix4x4>

const float step=0.01f; // шаг сдвига
float ratio;

Scene::Scene(QWidget *parent) :
    QOpenGLWidget (parent),
    m_angle(0),
    perspective(false)
{
  // задаём для виджета фокус, чтобы оно реагировало на кнопки
  this->setFocusPolicy(Qt::StrongFocus);
  // подключаем таймер к слоту
  connect(&m_timer,SIGNAL(timeout()),this,SLOT(slotAnimation()));
  // запускаем таймер
  m_timer.start(20);

  /*/ Specify the format and create platform-specific surface // В ATI ломает рисовку
  QSurfaceFormat format;
  format.setDepthBufferSize( 24 );
  format.setSamples( 4 );
  format.setStencilBufferSize(8);
  //format.setVersion(2,0);
  //format.setProfile( QSurfaceFormat::CompatibilityProfile );//( QSurfaceFormat::NoProfile ); // NoProfile for OGL<3.2 ( QSurfaceFormat::CoreProfile ); //// ( QSurfaceFormat::CompatibilityProfile )
  setFormat( format );//*/
}

Scene::~Scene()
{
makeCurrent();      // тут используется текущий контекст системы, т.к. QOpenGLWidget::currentContext() уже нет
delete m_triangle;
    delete spherepoints;
doneCurrent();
}

void Scene::initializeGL() {
       QOpenGLFunctions_3_0 *f = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_0>();
                if ( !f ) {
                    qWarning( "Could not obtain OpenGL versions object&quot" );
                    exit( 1 );
                }
            f->initializeOpenGLFunctions();

    //
    qDebug() << QString((const char*)f->glGetString(GL_VERSION)) << "\n" <<
                QString((const char*)f->glGetString(GL_VENDOR))<< "\n" <<
                QString((const char*)f->glGetString(GL_RENDERER));//<< "\n" << glGetString(GL_EXTENTIONS);

    qDebug() << "Version format= " << QOpenGLContext::currentContext()->format().majorVersion() <<
                " " << QOpenGLContext::currentContext()->format().minorVersion();

    // очищаем поле
    //glClearColor(0.1f,0.1f,0.2f,1.0f); // тёмно-синенький
    f->glClearColor(0.1f,0.1f,0.2f,1.0f); //

    //создаём объект - треугольник (создаём один, а нарисовать можем много таких-же)
    //m_triangle=new Triangle(&m_program, m_vertexAttr, m_colorAttr, m_texAttr, m_texUniform);
    m_triangle=new Triangle();

    // инициализируем массив координат для сетевой сферы
    spherepoints=new pointsofsphere(300);

    // тест - создаём простую сферу на шейдерах
    m_shphere=new shphere();
    if (perspective)
        m_shphere->setPerspective();
            else
    m_shphere->setOrthogonal();
}

void Scene::paintGL(){
    setStates();                    // включаем буфер глубины, свет и прочее (возможно можно вынести в initGL)

    //glClear(GL_COLOR_BUFFER_BIT); // если включен буфер глубины, то без его очистки мы ничего не увидим
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //glMatrixMode(GL_PROJECTION);
    //qgluPerspective(60.0, width / height, 0.01, 15.0);

    //glMatrixMode(GL_MODELVIEW);

    //пишем матрицы преобразований
    QMatrix4x4 LMM; // Local Model matrix (делает преобразования в локальных координатах объекта, для одного объекта их может быть несколько для разных частей объекта)
    QMatrix4x4 MM; // Model matrix (выносит координаты объекта в координаты пространства сцены,
                  //выполняется в следующем порядке - масштабирование, поворот, перенос)
                  // TranslationMatrix * RotationMatrix * ScaleMatrix * OriginalVector; (в коде это выглядит в обратном порядке)
    QMatrix4x4 MVM; // ModelView matrix (View matrix)("масштабирует крутит и перемещает весь мир")
    QMatrix4x4 ModelView; // тоже самое что и MVM, но для использования функции LookAt
    QMatrix4x4 PM; // Projection matrix // проекционная матрица
    QMatrix4x4 MVPM; // ModelViewProjection matrix (projection * view * model)
    if (perspective) {
        // устанавливаем трёхмерную канву (в перспективной проекции) для рисования (плоскости отсечения)
        // угол перспективы, отношение сторон, расстояние до ближней отсекающей плоскости и дальней
        PM.perspective(30.0f,ratio,0.1f,100.0f);  // glFrustum( xmin, xmax, ymin, ymax, near, far)  // gluPerspective(fovy, aspect, near, far)
    }
    else {
        // устанавливаем трёхмерную канву (в ортогональной проекции) для рисования (плоскости отсечения)
        PM.ortho(-2.0f,2.0f,-2.0f,2.0f,-8.0f,8.0f); // glOrtho(left,right,bottom,top,near,far) // увеличение значений уменьшает фигуры на сцене (по Z задаём больше, чтобы не видеть отсечение фигур)
        // переносим по z дальше, обязательное условие для перспективной проекции // по оси z 0 это "глаз", - движение камеры назад, + вперёд.
    }
    ///MVM.translate(0.0f,0.0f,-6.0f); // переносим по z от "глаз", сдвигаем камеру на минус, т.е. в сторону затылка.
    // не работает в ортогональной проекции если перенести слишком далеко, за пределы куба отсечения
    // оппа, мы видим передние границы пирамиды отсечения, где всё отсекается (тут-то шейдерным сферам и конец)
    // изменяем масштаб фигуры (увеличиваем)
    ///MVM.scale(10.0f);  // отрицательное число переворачивает проекцию // влияет только на ортогональную проекцию // убивает Шферы
    // указываем угол поворота и ось поворота смотрящую из центра координат к точке x,y,z,
    MVM.rotate(m_angle,0.0f,1.0f,0.0f);
    //MVM.lookAt();

    // находим проекционную инверсную мтрицу
    bool inverted;
    QMatrix4x4 PMi=PM.inverted(&inverted);
    if (!inverted)
        qDebug() << "PMi не конвертится";
    MVPM=PM*MVM;
    QMatrix4x4 MVPMi=MVPM.inverted(&inverted);
    if (!inverted)
        qDebug() << "MVPMi не конвертится";

    // РИСУЕМ ТРЕУГОЛЬНИК
    // инициализируем данные программы матрицы и атрибуты
    m_triangle->init();
    // зaпихиваем в его программу матрицу ориентации
    m_triangle->m_program.setUniformValue(m_triangle->m_matrixUniform, MVPM);
    // вызываем функцию рисования объекта (или объектов в for)
    m_triangle->draw();
    // проводим сброс инициализации параметров
    m_triangle->drop();//*/

    //РИСУЕМ СФЕРЫ
    m_shphere->init();
    m_shphere->m_program->setUniformValue(m_shphere->m_matrixUniform, MVPM);
    m_shphere->m_program->setUniformValue("PMi", PMi);
    m_shphere->m_program->setUniformValue("MVM", MVM);
    m_shphere->m_program->setUniformValue("MVPMi", MVPMi);
    m_shphere->m_program->setUniformValue("viewport",viewport);
    m_shphere->draw();
    m_shphere->drop();//*/

}

void Scene::resizeGL(int w, int h){
  ratio = (1.0*w)/(!h?1:h);
  glViewport(0,0,w,h);
  viewport.setX(0); viewport.setY(0); viewport.setZ((float)w); viewport.setW((float)h);
}

void Scene::setStates()
{
    // glEnable(GL_BRAINS) // ;)

    // glEnable(GL_BLEND); // На старых картах ATI вызывает проблемы в шейдере (цветовая глубина в профиле для блендинга должна быть 32 т.е. с альфой)

    glEnable(GL_DEPTH_TEST);  // без этого у нас не будет эффекта глубины (включаем буфер глубины)
    //glEnable(GL_CULL_FACE); // активирует устранение спрятанных поверхностей.
    glEnable(GL_LIGHTING);
    glEnable(GL_COLOR_MATERIAL);  // было отключено
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_NORMALIZE);

    setLights();

    float materialSpecular[] = {0.5f, 0.5f, 0.5f, 1.0f};
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, materialSpecular);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 32.0f);
}

void Scene::setLights()
{
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    float lightColour[] = {1.0f, 0.9f, 0.9f, 1.0f};

    ///glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColour);

    float lightDir[] = {1.0f, 1.0f, 1.0f, 0.0f};
    //glLightfv(GL_LIGHT0, GL_SPECULAR, lightColour);
    glLightfv(GL_LIGHT0, GL_POSITION, lightDir);

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

    // deprecated
    //glMatrixMode(GL_MODELVIEW);
    //glPopMatrix();
    //glMatrixMode(GL_PROJECTION);
    //glPopMatrix();

    glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, 0.0f);
    float defaultMaterialSpecular[] = {0.0f, 0.0f, 0.0f, 1.0f};
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, defaultMaterialSpecular);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0.0f);
}

void Scene::setOrthogonal()
{
    perspective=false;
    m_shphere->setOrthogonal();
}

void Scene::setPerspective()
{
    perspective=true;
    m_shphere->setPerspective();
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
