#include "shphere.h"
/// класс рисования сферы с помощью шейдера
/// пока только в перспективной проекции (?)
///

shphere::shphere()
{

}

/*// получение лога ошибок компиляции шейдеров
void shphere::printInfoLog(GLuint obj) {
    int log_size = 0;
    int bytes_written = 0;
    glGetProgramiv(obj, GL_INFO_LOG_LENGTH, &log_size);
    if (!log_size) return;
    char *infoLog = new char[log_size];
    glGetProgramInfoLog(obj, log_size, &bytes_written, infoLog);
    std::cerr << infoLog << std::endl;
    delete [] infoLog;
}//*/

void shphere::draw()
{
    // устанавливаем место хранения координат
      m_program->setAttributeArray(m_vertexAttr, m_vertices.data(), 3);
      m_program->setAttributeArray(m_colorAttr, m_colors.data(), 3);
      m_program->setAttributeArray(m_texAttr, m_texcoords.data(), 2);
      m_program->setUniformValue(m_texAttr,0);

      // активируем массивы
      m_program->enableAttributeArray(m_vertexAttr);
      m_program->enableAttributeArray(m_colorAttr);
      m_program->enableAttributeArray(m_texAttr);

      // рисуем треугольник
      glDrawArrays(GL_TRIANGLES,0,m_vertices.size()/3);

      // деактивируем массивы
      m_program->disableAttributeArray(m_vertexAttr);
      m_program->disableAttributeArray(m_colorAttr);
      m_program->disableAttributeArray(m_texAttr);
}

