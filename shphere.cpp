#include "shphere.h"
/// класс рисования сферы с помощью шейдера
/// пока только в перспективной проекции (?)
///

/*// получение лога ошибок компиляции шейдеров
void shphere::printInfoLog(GLuint obj) {
    int log_size = 0;
    int bytes_written = 0;
    glGetProgramiv(obj, GL_INFO_LOG_LENGTH, &log_size);
    if (!log_size) return;
    char *infoLog = new char[log_size];
    glGetProgramInfoLog(obj, log_size, &bytes_written, infoLog);
    std::cerr << infoLog << std::endl;
    delete [] infoLog;
}//*/

shphere::shphere():
    m_x0(-0.05f),
    m_y0(-0.05f),
    m_z0(0.0f),
    m_dist(0.1f)  // distations beatween shpheres
{
    // инициализируем шейдеры
    QOpenGLShader vShader(QOpenGLShader::Vertex);
    //vShader.compileSourceFile(":/Shaders/vShphere.glsl");

    QOpenGLShader fShader(QOpenGLShader::Fragment);
    //fShader.compileSourceFile(":/Shaders/fShphere.glsl");

    //добавляем шейдеры в программу для ортогональной проекции
    vShader.compileSourceFile(":/Shaders/vShphereOrto.glsl");
    fShader.compileSourceFile(":/Shaders/fShphereOrto.glsl");
    //vShader.compileSourceFile(":/Shaders/vShader.glsl");
    //fShader.compileSourceFile(":/Shaders/fShader.glsl");
    m_programO.addShader(&vShader);
    m_programO.addShader(&fShader);
    // линкуем загруженные в программу шейдеры вместе и проверяем
    if (!m_programO.link()){
        qWarning("Хъюстон, у нас проблемы: Шейдерная программа для Шфер не слинковалась");
        return; // Хъюстон, у нас проблемы
    }
    //setOrthogonal(); // инициализируем на всякий случай, переопределяется в Scene::initializeGL()

    //добавляем шейдеры в программу для перспективной проеции
    vShader.compileSourceFile(":/Shaders/vShphereProection.glsl");
    fShader.compileSourceFile(":/Shaders/fShphereProection.glsl");
    m_programP.addShader(&vShader);
    m_programP.addShader(&fShader);
    if (!m_programP.link()){
        qWarning("Шейдерная программа для перспективы не слинковалась");
        return;
    }
    //setPerspective();//*/ // инициализируем на всякий случай, переопределяется в Scene::initializeGL()

    initVertices();
    initColors();
}

void shphere::init()
{
    //подключаем программу (glUseProgram) и проверяем
    if (!m_program->bind()){
        qWarning("Хъюстон, у нас проблемы: Шейдерная программа не сбиндилась");
        return;
    }
}

void shphere::draw()
{
   // устанавливаем место хранения координат
      m_program->setAttributeArray(m_vertexAttr, m_vertices.data(), 3);
      m_program->setAttributeArray(m_colorAttr, m_colors.data(), 3);
      //m_program->setAttributeArray(m_texAttr, m_texcoords.data(), 2);
      //m_program->setUniformValue(m_texAttr,0);

      //m_program->setAttributeValue("R",R); // пока используем статичный радиус
      m_program->setUniformValue("R",0.01f); // пока используем статичный радиус

      // активируем массивы цветов
      m_program->enableAttributeArray(m_vertexAttr);
      m_program->enableAttributeArray(m_colorAttr);
      //m_program->enableAttributeArray(m_texAttr);
      //qDebug() << m_program.log();

      // получаем текущий viewport (как будет решаться вопрос с поворотом???)
      float viewport2[4];
      glGetFloatv(GL_VIEWPORT, viewport2);  //получаем размеры окна рисования (x0,y0,w,h)
      //glUniform4fv(m_program.programId(),1,viewport2);
      QVector4D viewport(viewport2[0],viewport2[1],viewport2[2],viewport2[3]);
      ///m_program->setUniformValue("viewport",viewport);

      // рисуем точки
      glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);   // говорим что будут меняться размеры точек в шейдере

      glDrawArrays(GL_POINTS,0,m_vertices.size()/3);

      glDisable(GL_VERTEX_PROGRAM_POINT_SIZE);

      // деактивируем массивы
      m_program->disableAttributeArray(m_vertexAttr);
      m_program->disableAttributeArray(m_colorAttr);
      //m_program->disableAttributeArray(m_texAttr);//*/
}

void shphere::drop()
{
    // очищаем программу
    m_program->release();
}

void shphere::initVertices()   // инициализация вектора вершин
{
m_vertices.resize(9); // увеличиваем масив до 9 значений, т.к. у нас 3 вершины
// 0
m_vertices[0] = m_x0;
m_vertices[1] = m_y0;
m_vertices[2] = m_z0;

// 1
m_vertices[3] = m_x0+m_dist;
m_vertices[4] = m_y0;
m_vertices[5] = m_z0;

// 2
m_vertices[6] = m_x0+m_dist/2.0f;
m_vertices[7] = m_y0+m_dist;
m_vertices[8] = m_z0;
}

// на каждую точку оформляем по одной координате цвета (R,G,B)
void shphere::initColors()
{
    m_colors.resize(9); // увеличиваем масив до 9 значений, т.к. у нас 3 вершины по 3 цвета в каждой (или 12 если с альфой)
    // 0
    m_colors[0] = 1.0f;
    m_colors[1] = 0.0f;
    m_colors[2] = 0.0f;

    // 1
    m_colors[3] = 0.0f;
    m_colors[4] = 1.0f;
    m_colors[5] = 0.0f;

    // 2
    m_colors[6] = 0.0f;
    m_colors[7] = 0.0f;
    m_colors[8] = 1.0f;
}

void shphere::setOrthogonal()
{
    m_program=&m_programO;
    // устанавливаем привязку между приложением и шейдерами  ???зачем???
    // возможно так быстрее будет обращаться к переменным напрямую, чем по имени, если нет, можно обойтись без этого блока
    m_vertexAttr=m_program->attributeLocation("vertexAttr");          // координаты точек из массива
    m_matrixUniform=m_program->uniformLocation("MVPM");  // область просмотра
    m_colorAttr=m_program->attributeLocation("colorAttr");           // соответствующий набор цветов для точек из массива
    //m_texAttr=m_program->attributeLocation("texAttr");
    //m_texUniform=m_program->attributeLocation("texUniform");//*/
}

void shphere::setPerspective()
{
    m_program=&m_programP;
    // устанавливаем привязку между приложением и шейдерами  ???зачем???
    // возможно так быстрее будет обращаться к переменным напрямую, чем по имени, если нет, можно обойтись без этого блока
    m_vertexAttr=m_program->attributeLocation("vertexAttr");          // координаты точек из массива
    m_matrixUniform=m_program->uniformLocation("MVPM");  // область просмотра
    m_colorAttr=m_program->attributeLocation("colorAttr");           // соответствующий набор цветов для точек из массива
    //m_texAttr=m_program->attributeLocation("texAttr");
    //m_texUniform=m_program->attributeLocation("texUniform");//*/
}

void shphere::setx0(float x)
{
    m_x0=x;
    // пересчитываем остальные точки (инициализируем заново вершины)
    initVertices();
}

void shphere::sety0(float y)
{
    m_y0=y;
    // пересчитываем остальные точки (инициализируем заново вершины)
    initVertices();
}

void shphere::setz0(float z)
{
    m_z0=z;
    // пересчитываем остальные точки (инициализируем заново вершины)
    initVertices();
}

QString shphere::getFigureInfo()
{
    QString text="Стартовая точка Шферы: x="+QString().setNum(m_x0)+","+QString().setNum(m_y0)+","+QString().setNum(m_z0);
    return text;  // так как переменная уничтожается после выхода из функции, надо использовать спецификатор const
}

