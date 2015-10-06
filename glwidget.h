#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QCoreApplication>
#include <QDebug>
#include <QOpenGLFunctions_3_3_Core>

#include <QGLWidget>

#include <QOpenGLWidget>
#include <QOpenGLShader>
#include <QKeyEvent>

#include <QGLBuffer>
#include <QGLShaderProgram>

class GLWidget : public QGLWidget
{
 Q_OBJECT
public:
 GLWidget( const QGLFormat& format, QWidget* parent = 0 );

protected:
 virtual void initializeGL();
 virtual void resizeGL( int w, int h );
 virtual void paintGL();

virtual void keyPressEvent( QKeyEvent* e );

private:
 bool prepareShaderProgram( const QString& vertexShaderPath,
 const QString& fragmentShaderPath );

QGLShaderProgram m_shader;
 QGLBuffer m_vertexBuffer;
 QOpenGLFunctions_3_3_Core *glfuncs;
};

#endif // GLWIDGET_H
