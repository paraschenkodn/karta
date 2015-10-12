#ifndef SHPHERE_H
#define SHPHERE_H

#include <QOpenGLShaderProgram>  // объявляем для передачи шейдерных программ в объект

class shphere
{
public:
    shphere();
    void printInfoLog(GLuint obj); //получение лога ошибок компиляции шейдеров
    void draw(); // рисуем сферу (шейдерами)

    QOpenGLShaderProgram *m_program;
    int m_vertexAttr;
    int m_colorAttr;
    int m_texAttr;
    int m_texUniform;

};

#endif // SHPHERE_H
