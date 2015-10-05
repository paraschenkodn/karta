#include "openglwindow.h"

OpenGLWindow::OpenGLWindow(QWindow *parent) // создаём окно вызывая конструктор с инициализацией переменных.
    : QWindow(parent)
    , m_update_pending(false)       //
    , m_animating(false)            //
    , m_context(0)                  //
    , m_device(0)                   //
    , needsInitialize(false)        // состояние по умолчанию, когда поребуется инициализация точек входа функций OpenGL мы флаг взведём
{
    setSurfaceType(QWindow::OpenGLSurface); /// Необходимо указать Qt что мы для рисования в окне будем использовать OpenGL,
                                            /// а не использовать растровый контент отображаемый QPainter используя QBackingStore.
}

/// Установка формата отображения контекста производится в конструкторе дочерних окон. (Раз это окно не предназначено для непосредственного отображения)
/// Специфицируем формат и создаем платформо-зависимый сюрфейс
QSurfaceFormat format;
format.setDepthBufferSize(24);
format.setMajorVersion(4);
format.setMinorVersion(3);
format.setSamples(4);
format.setProfile(QSurfaceFormat::CoreProfile);
setFormat(format);
create();//*/               // привязываем формат к окну

/// инициализацию контекста будем производить при первом обращении к функции рисования поверхности,
/// т.к. событие обновления окна может быть вызвано в любой момент, даже когда ещё неизвестен ни размер окна, ни его видимость
/*// Создаем OpenGL контекст
m_context = new QOpenGLContext;
m_context->setFormat(format);
m_context->create();//*/

/// Если окно должно быть перерисовано, вызываем функцию рисования renderNow()
void OpenGLWindow::exposeEvent(QExposeEvent *event)
{
    Q_UNUSED(event);

    if (isExposed())        // проверяем статус окна и вызываем прорисовку при необходимости
        renderNow();
}
/// функции обработки сигналов через которые будет происходить прорисовка экрана следующие: renderNow() и renderLater()
/// renderNow()будет вызываться всякий раз когда поступит событие обновления эрана и заданное нами событие следующей прорисовки
/// которое устанавливается в renderLater() для задания эффекта анимации
void OpenGLWindow::renderNow()  // основная функция рисования с проверками инициализации OpenGL
{
    if (!isExposed())                               // Если окно не отображается или его часть, то нам тут делать нечего.
        return;

    if (!m_context) {                               // Здесь происходит инициализация контекста, если он ещё не инициализирован.
        m_context = new QOpenGLContext(this);
        m_context->setFormat(requestedFormat());    // Инициализируем пока в текущем формате, который будет переопределяться при необходимости в дочерних окнах
        m_context->create();                        // привязываем формат к контексту (??)

        needsInitialize = true;                     // устанавливаем флаг, что требуется инициализация точек входа к функциям OpenGL
    }

    m_context->makeCurrent(this);           // // Сделаем контекст текущим для этого окна

    if (needsInitialize) {                  //
        // Получить объект функции и назначить все точки входа
        // m_funcs объявлен как: QOpenGLFunctions_4_3_Core * m_funcs
        m_funcs = m_context->versionFunctions();
            if (!m_funcs) {
              qWarning("Не поддерживает установленная в систему версия OpenGL функции версии 4.3, \n обновите драйвера или карту");
              exit(1);  // Первая проблема у нас
            }
        m_funcs->initializeOpenGLFunctions();       // получаем доступ к функциям OpenGL 4.3 теперь в функции render() можем использовать функции OpenGL такие как glClear и т.д.

        initialize();                   // вызываем функцию инициализации
        needsInitialize=false;          // убираем флаг инициализации
    }

    render();                           // добрались до прорисовки сцены (там мы будем рисовать в буфере?)

    m_context->swapBuffers(this);       // выкидываем на экран из буфера то, что нарисовали

    if (m_animating)
        renderLater();
}

// Если функция не переопределена, то вызовется рисовальщик по умолчанию render(QPainter *) с соответствующей инициализацией
void OpenGLWindow::render()
{
    if (!m_device)
        m_device = new QOpenGLPaintDevice;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    m_device->setSize(size());

    QPainter painter(m_device);
    render(&painter);
}

// любая инициализация OpenGL , как и функция прорисовки может быть перекрыта в дочернем окне, поэтому эти виртуальные функции пустые,
// функция инициализации вызывается один раз перед вызовом прорисовки сцены,
// но при этом должен быть обеспечен правильный контекст, и проинициализировано устройство
// что обеспечивает по умолчанию функция render(), инициализирует устройство вывода и передаёт его указатель в render(QPainter *painter)
void OpenGLWindow::render(QPainter *painter)
{
    Q_UNUSED(painter);
}

void OpenGLWindow::initialize()
{
}

/// прорисовку анимации организовываем через очередь сообщений организованой посредством вызовов renderLater() и event(QEvent *)
void OpenGLWindow::renderLater()
{
    if (!m_update_pending) {
        m_update_pending = true;
        QCoreApplication::postEvent(this, new QEvent(QEvent::UpdateRequest));
    }
}

bool OpenGLWindow::event(QEvent *event)
{
    switch (event->type()) {
    case QEvent::UpdateRequest:
        m_update_pending = false;
        renderNow();
        return true;
    default:
        return QWindow::event(event);
    }
}

/// установка флага анимации задействует очередь сообщений окна посредством renderLater()
void OpenGLWindow::setAnimating(bool animating)
{
    m_animating = animating;

    if (animating)
        renderLater();
}

