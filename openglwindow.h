#ifndef OPENGLWINDOW_H
#define OPENGLWINDOW_H

#include <QWindow>
#include <QOpenGLPaintDevice>
#include <QOpenGLFunctions_4_3_Core>

// Создаём класс окна предназначенного для рисования в OpenGL
/// Этот класс окна будет являться родителем для окон которые мы будем создавать для рисования наследуя от него
/// В этом окне будут задаваться свойства и методы общие для дочерних окон
/// Смысл создания родительского класса - упростить код создаваемых дочерних окон и вынести в них платформонезависимый код.
// Qt обладает собственными механизмами инициализаци OpenGL, поэтому
// ручная инициализация библиотеками GLEW GLEXT GL и т.д., которую вы видите в многочисленных уроках
// вам не нужна в большинстве случаев, только если вы не планируете использовать самые последние версии OpenGL.
// Тогда вам придётся скомпилировать необходимые библиотеки самим и подключить их в проект, с ручной инициализацией OpenGL контекста.
// Всю эту машинерию за вас выполняют классы QOpenGLContext и QOpenGLPaintDevice
///
/// \brief The OpenGLWindow class
/// QOpenGLFunctions
/// QWindow
///
class OpenGLWindow : public QWindow ///, protected QOpenGLFunctions  //
{
    Q_OBJECT
public:
    explicit OpenGLWindow(QWindow *parent = 0);  // (explicit???)
    ~OpenGLWindow();

    virtual void render(QPainter *painter); // прорисовщик вызывающийся по умолчанию
    virtual void render();                  // наша функция рисования, которую мы переопределяем в дочерних окнах

    virtual void initialize();              // инициализация OpenGL (настраиваем общие параметры), которую мы переопределим в дочерних окнах

    void setAnimating(bool animating);      // ???

    QOpenGLFunctions_4_3_Core * m_funcs;    /// объявляем точку для обращения к функциям конкретной версии OpenGL (мы ведь хотим пользоваться последней?)
                                            /// Если бы нам требовалась совместимость мы могли бы просто использовать ... ", protected QOpenGLFunctions"  в объявлении
                                            /// класса и не объявлять данную переменную.
public slots:
    void renderLater();     //
    void renderNow();       //

protected:
    bool event(QEvent *event) Q_DECL_OVERRIDE;                  //

    void exposeEvent(QExposeEvent *event) Q_DECL_OVERRIDE;      //

private:
    bool m_update_pending;          // флаг необходимости обновления сцены (прорисовки буфера)
    bool m_animating;               // флаг запуска анимации
    bool needsInitialize;           // устанавливаем флаг инициализации точек входа функций OpenGL

    QOpenGLContext *m_context;              //
    QOpenGLPaintDevice *m_device;           //
};

#endif // OPENGLWINDOW_H
