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
    // устанавливаем признак рисования в ортогональной проекции
    void setOrthogonal();
    // устанавливаем признак рисования в перспективной проекции
    void setPerspective();

    QOpenGLShaderProgram *m_program, m_programO, m_programP;
    //GLuint m_progID;
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
