#ifndef SHPHERE_H
#define SHPHERE_H

#include <QOpenGLShaderProgram>  // объявляем для передачи шейдерных программ в объект

class shphere
{
public:
    shphere();
    void printInfoLog(GLuint obj); //получение лога ошибок компиляции шейдеров
    void init();
    void draw(); // рисуем сферу (шейдерами)
    void drop();

    QOpenGLShaderProgram m_program;
    int m_vertexAttr;
    int m_colorAttr;
    int m_texAttr;
    int m_texUniform;
    int m_matrixUniform;

private:
    std::vector<float> m_vertices; //создаём вектор (контейнер хранения) координат вершин
    std::vector<float> m_colors; //создаём вектор (контейнер хранения) координат цветов точек которые рисуем

    void initVertices();
    void initColors();      // инициализация вектора цветов
};

#endif // SHPHERE_H
