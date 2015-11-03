#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <vector>  // подключаем объекты класса vector
#include <QOpenGLShaderProgram>  // объявляем для передачи шейдерных программ в объект
#include <QOpenGLTexture>

class Triangle
{
public:
    Triangle(QOpenGLShaderProgram *program,
             int vertexAttr, int colorAttr, int texAttr, int texUniform);
    Triangle();

    ~Triangle();

    // инициализация переменных и шейдеров (инициализируем один раз непосредственно перед рисованием одного или множества одинаковых объектов)
    void init();  // open()
    // метод рисования  (рисуем один или множество одинаковых объектов)
    void draw();
    // деинициализация программ (шейдеров), объекотв и переменных
    void drop();    // close()

    void setx0(float x);
    void sety0(float y);
    void setz0(float z);

    float getx0();
    float gety0();
    float getz0();

    //координаты начальной позиции треугольника
    float m_x0;
    float m_y0;
    float m_z0;
    float m_size;
    QString getFigureInfo();

    QOpenGLShaderProgram m_program;


    int m_vertexAttr;
    int m_colorAttr;
    int m_texAttr;
    int m_texUniform;
    int m_matrixUniform;

private:
    std::vector<float> m_vertices; //создаём вектор (контейнер хранения) координат вершин
    std::vector<float> m_colors; //создаём вектор (контейнер хранения) координат цветов
    std::vector<float> m_texcoords;
    void initVertices();   // инициализация вектора вершин
    void initColors();      // инициализация вектора цветов
    void initTexCoords();  // инициализация текстурных координат

    QOpenGLTexture *m_texture;
};

#endif // TRIANGLE_H
