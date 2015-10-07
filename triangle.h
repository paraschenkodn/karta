#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <vector>  // подключаем объекты класса vector
#include <QOpenGLShaderProgram>  // объявляем для передачи шейдерных программ в объект

class Triangle
{
public:
    Triangle(QOpenGLShaderProgram *program,
             int vertexAttr, int colorAttr);

    // метод рисования
    void draw();

private:
    std::vector<float> m_vertices; //создаём вектор (контейнер хранения) координат вершин
    std::vector<float> m_colors; //создаём вектор (контейнер хранения) координат цветов
    void initVertices();   // инициализация вектора вершин
    void initColors();      // инициализация вектора цветов

    QOpenGLShaderProgram *m_program;
    int m_vertexAttr;
    int m_colorAttr;

};

#endif // TRIANGLE_H
