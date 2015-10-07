#include "scene.h"

#include <QMatrix4x4>

Scene::Scene(QWidget *parent) :
    QOpenGLWidget (parent)
{

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

    //создаём объект - треугольник
    m_triangle=new Triangle(&m_program, m_vertexAttr, m_colorAttr);
}

void Scene::paintGL(){
    glClear(GL_COLOR_BUFFER_BIT);
    //подключаем программу и проверяем
    if (!m_program.bind()){
        qWarning("Хъюстон, у нас проблемы:\nШейдерная программа не сбиндилась");
        return;
    }
    //пишем матрицу ориентации
    QMatrix4x4 matrix;
    // устанавливаем трёхмерную канву для рисования (плоскости отсечения)
    matrix.ortho(-2.0f,2.0f,-2.0f,2.0f,2.0f,-2.0f);
    // переносим по y дальше
    matrix.translate(0.0f,0.0f,-1.0f);
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
