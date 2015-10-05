#ifndef LENSWINDOW_H
#define LENSWINDOW_H

#include <openglwindow.h>
///
/// \brief The LensWindow class
///
class LensWindow:public OpenGLWindow
{
public:
    LensWindow();

    void initialize() Q_DECL_OVERRIDE;  // переопределяем функцию родительского окна, чтобы внести дополнительные параметры инициализации
    void render() Q_DECL_OVERRIDE;      // переопределяем функцию рисования. При первом обращении к этой функции происходит инициализация контекста
                                        // определяемая в родительском окне.

private:
    GLuint loadShader(GLenum type, const char *source);

    GLuint m_posAttr;
    GLuint m_colAttr;
    GLuint m_matrixUniform;

    QOpenGLShaderProgram *m_program;
    int m_frame;
};

#endif // LENSWINDOW_H
