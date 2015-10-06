#include "glwidget.h"

/// шейдеры для рисования (определяем статически, можно конечно же потом грузить из файла)
static const char *vertexShaderSource =
        "#version 330 core\n"
        "uniform mat4 projectionMatrix;\n"
        "in vec3 position;\n"
        "in vec3 color;\n"
        "out vec3 fragmentColor;\n"
        "void main(void) {\n"
                // перевод вершинных координат в однородные
                "gl_Position   = projectionMatrix * vec4(position, 1.0);\n"
                // передаем цвет вершины в фрагментный шейдер
                "fragmentColor = color; }\n";//*/

static const char *fragmentShaderSource =
        "#version 330 core\n"
        "in vec3 fragmentColor;\n"
        "out vec4 color;\n"
        "void main(void) {\n"
                // зададим цвет пикселя
                "color = vec4(fragmentColor, 1.0); }\n";//*/

GLWidget::GLWidget( const QGLFormat& format, QWidget* parent )
 : QGLWidget( format, parent ),
 m_vertexBuffer( QGLBuffer::VertexBuffer )
{
}

void GLWidget::initializeGL()
{
 QGLFormat glFormat = QGLWidget::format();
 if ( !glFormat.sampleBuffers() )
 qWarning() << "Could not enable sample buffers";

 // Set the clear color to black
 glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );

 // Prepare a complete shader program…
 if ( !prepareShaderProgram( ":/simple.vert", ":/simple.frag" ) )
 return;

 // We need us some vertex data. Start simple with a triangle ;-)
 float points[] = {-0.5f, -0.5f, 0.0f, 1.0f,
 0.5f, -0.5f, 0.0f, 1.0f,
 0.0f, 0.5f, 0.0f, 1.0f};
 m_vertexBuffer.create();
 m_vertexBuffer.setUsagePattern( QGLBuffer::StaticDraw );
 if ( !m_vertexBuffer.bind() )
 {
 qWarning() << "Could not bind vertex buffer to the context";
 return;
 }
 m_vertexBuffer.allocate( points, 3 * 4 * sizeof( float ) );

 // Bind the shader program so that we can associate variables from
 // our application to the shaders
 if ( !m_shader.bind() )
 {
 qWarning() << "Could not bind shader program to context";
 return;
 }

 // Enable the "vertex" attribute to bind it to our currently bound
 // vertex buffer.
 m_shader.setAttributeBuffer( "vertex", GL_FLOAT, 0, 4 );
 m_shader.enableAttributeArray( "vertex" );

 /*glfuncs = m_context->versionFunctions();
 if (!glfuncs) {
   qDebug()<<"Не поддерживается версия OpenGL этой программой.";
   exit(1); //Хъюстон, у нас проблема номер раз
 }
 glfuncs->initializeOpenGLFunctions();

 uint vao;
 glGenVertexArrays(1, &vao);
 glBindVertexArray(vao);//*/

 qDebug() << QString((const char*)glGetString(GL_VERSION)) << "\n" << QString((const char*)glGetString(GL_VENDOR))<< "\n" << QString((const char*)glGetString(GL_RENDERER));//<< "\n" << glGetString(GL_EXTENTIONS);

}

bool GLWidget::prepareShaderProgram( const QString& vertexShaderPath,
 const QString& fragmentShaderPath )
{
 // First we load and compile the vertex shader…
 bool result = m_shader.addShaderFromSourceFile( QGLShader::Vertex, vertexShaderPath );
 //bool result = m_shader.addShaderFromSourceCode(QGLShader::Vertex, vertexShaderSource);
 if ( !result )
 qWarning() << m_shader.log();

// …now the fragment shader…
 result = m_shader.addShaderFromSourceFile( QGLShader::Fragment, fragmentShaderPath );
 //result = m_shader.addShaderFromSourceCode(QGLShader::Fragment, fragmentShaderSource);
 if ( !result )
 qWarning() << m_shader.log();

// …and finally we link them to resolve any references.
 result = m_shader.link();
 if ( !result )
 qWarning() << "Could not link shader program:" << m_shader.log();

return result;
}

void GLWidget::resizeGL( int w, int h )
{
 // Set the viewport to window dimensions
 glViewport( 0, 0, w, qMax( h, 1 ) );
}

void GLWidget::paintGL()
{
 // Clear the buffer with the current clearing color
 glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

// Draw stuff
 glDrawArrays( GL_TRIANGLES, 0, 3 );
}

void GLWidget::keyPressEvent( QKeyEvent* e )
{
 switch ( e->key() )
 {
 case Qt::Key_Escape:
 QCoreApplication::instance()->quit();
 break;

default:
 QGLWidget::keyPressEvent( e );
 }
}

