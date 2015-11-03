#include "triangle.h"

Triangle::Triangle(QOpenGLShaderProgram *program, int vertexAttr, int colorAttr, int texAttr, int texUniform):
        m_program(program),
        m_vertexAttr(vertexAttr),
        m_colorAttr(colorAttr),
        m_texAttr(texAttr),
        m_texUniform(texUniform),
        m_x0(-0.05f),
        m_y0(-0.05f),
        m_z0(0.0f),
        m_size(0.1f)  // size triangle
{
initVertices();
initColors();
initTexCoords();
// грузим текстуру
m_texture=new QOpenGLTexture(QImage(":/Textures/rbt.png"));
}

Triangle::Triangle():
    m_x0(-0.05f),
    m_y0(-0.05f),
    m_z0(0.0f),
    m_size(0.1f)  // size triangle
{
    initVertices();
    initColors();
    initTexCoords();
    // грузим текстуру
    m_texture=new QOpenGLTexture(QImage(":/Textures/rbt.png"));

    // инициализируем шейдеры
    QOpenGLShader vShader(QOpenGLShader::Vertex);
    vShader.compileSourceFile(":/Shaders/vShader.glsl");

    QOpenGLShader fShader(QOpenGLShader::Fragment);
    fShader.compileSourceFile(":/Shaders/fShader.glsl");

    //добавляем шейдеры в программу
    m_program.addShader(&vShader);
    m_program.addShader(&fShader);
    // линкуем загруженные в программу шейдеры вместе и проверяем
    if (!m_program.link()){
        qWarning("Хъюстон, у нас проблемы:\nШейдерная программа не слинковалась");
        return; // Хъюстон, у нас проблемы
    }
    // устанавливаем привязку между приложением и шейдерами  ???зачем???
    // возможно так быстрее будет обращаться к переменным напрямую, чем по имени, если нет, можно обойтись без этого блока
    m_vertexAttr=m_program.attributeLocation("vertexAttr");
    m_colorAttr=m_program.attributeLocation("colorAttr");
    m_matrixUniform=m_program.uniformLocation("MVPM");
    m_texAttr=m_program.attributeLocation("texAttr");
    m_texUniform=m_program.attributeLocation("texUniform");//*/

}

Triangle::~Triangle()
{
  delete m_texture;
}

void Triangle::init()
{
    //подключаем программу и проверяем
    if (!m_program.bind()){
        qWarning("Хъюстон, у нас проблемы:\nШейдерная программа не сбиндилась");
        return;
    }
    // убираем искажения текстур
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // подключаем текстуру   (наложение, или нет, текстуры, реализуется в самом шейдере)
    m_texture->bind();

}

void Triangle::draw()   // draw(*allcoords); // vertex, color, textures
{
  // устанавливаем место хранения координат
    m_program.setAttributeArray(m_vertexAttr, m_vertices.data(), 3);
    m_program.setAttributeArray(m_colorAttr, m_colors.data(), 3);
    m_program.setAttributeArray(m_texAttr, m_texcoords.data(), 2);
    m_program.setUniformValue(m_texAttr,0);

    // активируем массивы
    m_program.enableAttributeArray(m_vertexAttr);
    m_program.enableAttributeArray(m_colorAttr);
    m_program.enableAttributeArray(m_texAttr);//*/

    // рисуем треугольник
    glDrawArrays(GL_TRIANGLES,0,m_vertices.size()/3);

    // деактивируем массивы
    m_program.disableAttributeArray(m_vertexAttr);
    m_program.disableAttributeArray(m_colorAttr);
    m_program.disableAttributeArray(m_texAttr);
}

void Triangle::drop()
{
    // очищаем программу
    m_program.release();

}

void Triangle::setx0(float x)
{
  m_x0=x;
  // пересчитываем остальные точки (инициализируем заново вершины)
  initVertices();
}

void Triangle::sety0(float y)
{
  m_y0=y;
  // пересчитываем остальные точки (инициализируем заново вершины)
  initVertices();
}

void Triangle::setz0(float z)
{
    m_z0=z;
    // пересчитываем остальные точки (инициализируем заново вершины)
    initVertices();
}

float Triangle::getx0()
{
   return m_x0;
}

float Triangle::gety0()
{
    return m_y0;
}

float Triangle::getz0()
{
    return m_z0;
}

QString Triangle::getFigureInfo()
{
    QString text="Стартовая точка треугольника: x="+QString().setNum(m_x0)+","+QString().setNum(m_y0)+","+QString().setNum(m_z0);
    return text;
}

void Triangle::initVertices()   // инициализация вектора вершин
{
m_vertices.resize(9); // увеличиваем масив до 9 значений, т.к. у нас 3 вершины по 4 координаты в каждой (4?)
// 0
m_vertices[0] = m_x0;
m_vertices[1] = m_y0;
m_vertices[2] = m_z0;

// 1
m_vertices[3] = m_x0+m_size;
m_vertices[4] = m_y0;
m_vertices[5] = m_z0;

// 2
m_vertices[6] = m_x0+m_size/2.0f;
m_vertices[7] = m_y0+m_size;
m_vertices[8] = m_z0;
}

void Triangle::initColors()      // инициализация вектора цветов
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

void Triangle::initTexCoords()
{
  // задаём координаты на текстуре в порядке обхода вершин
  m_texcoords.resize(6);
  m_texcoords[0]= 0.0f;
      m_texcoords[1]=0.0f;
      m_texcoords[2]=1.0f;
      m_texcoords[3]=0.0f;
      m_texcoords[4]=0.5f;
      m_texcoords[5]=1.0f;
}
