#include "lenswindow.h"

/// шейдеры для рисования (определяем статически, можно конечно же потом грузить из файла)
static const char *vertexShaderSource =
    "attribute highp vec4 posAttr;\n"
    "attribute lowp vec4 colAttr;\n"
    "varying lowp vec4 col;\n"
    "uniform highp mat4 matrix;\n"
    "void main() {\n"
    "   col = colAttr;\n"
    "   gl_Position = matrix * posAttr;\n"
    "}\n";

static const char *fragmentShaderSource =
    "varying lowp vec4 col;\n"
    "void main() {\n"
    "   gl_FragColor = col;\n"
    "}\n";

LensWindow::LensWindow()
  : m_program(0)
  , m_frame(0)
{
/// инициализацию или изменение формата контекста можно вынести в main, при необходимости
    // Специфицируем формат и создаем платфоро-зависимый сюрфейс
    QSurfaceFormat format;
    format.setDepthBufferSize(24);      //
    format.setMajorVersion(4);          //
    format.setMinorVersion(3);          //
    format.setSamples(16);              //
    format.setProfile(QSurfaceFormat::CoreProfile);
    setFormat(format);
    create();
/// енд инициализацию формата контекста
}

GLuint LensWindow::loadShader(GLenum type, const char *source)
{
    GLuint shader = OpenGLWindow::m_funcs->glCreateShader(type);
    OpenGLWindow::m_funcs->glShaderSource(shader, 1, &source, 0);
    OpenGLWindow::m_funcs->glCompileShader(shader);
    return shader;
}

void LensWindow::initialize()
{
    m_program = new QOpenGLShaderProgram(this);
    m_program->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource);
    m_program->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource);
    m_program->link();
    m_posAttr = m_program->attributeLocation("posAttr");
    m_colAttr = m_program->attributeLocation("colAttr");
    m_matrixUniform = m_program->uniformLocation("matrix");
}

void LensWindow::render()
{
    const qreal retinaScale = devicePixelRatio();
    OpenGLWindow::m_funcs->glViewport(0, 0, width() * retinaScale, height() * retinaScale);

    OpenGLWindow::m_funcs->glClear(GL_COLOR_BUFFER_BIT);

    m_program->bind();

    QMatrix4x4 matrix;
    matrix.perspective(60.0f, 4.0f/3.0f, 0.1f, 100.0f);
    matrix.translate(0, 0, -2);
    matrix.rotate(100.0f * m_frame / screen()->refreshRate(), 0, 1, 0);

    m_program->setUniformValue(m_matrixUniform, matrix);

    GLfloat vertices[] = {
        0.0f, 0.707f,
        -0.5f, -0.5f,
        0.5f, -0.5f
    };

    GLfloat colors[] = {
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f
    };

    OpenGLWindow::m_funcs->glVertexAttribPointer(m_posAttr, 2, GL_FLOAT, GL_FALSE, 0, vertices);
    OpenGLWindow::m_funcs->glVertexAttribPointer(m_colAttr, 3, GL_FLOAT, GL_FALSE, 0, colors);

    OpenGLWindow::m_funcs->glEnableVertexAttribArray(0);
    OpenGLWindow::m_funcs->glEnableVertexAttribArray(1);

    OpenGLWindow::m_funcs->glDrawArrays(GL_TRIANGLES, 0, 3);

    OpenGLWindow::m_funcs->glDisableVertexAttribArray(1);
    OpenGLWindow::m_funcs->glDisableVertexAttribArray(0);

    m_program->release();

    ++m_frame;
}
