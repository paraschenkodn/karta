attribute highp vec4 vertexAttr; // входной аттрибут для координат вершин
uniform mediump mat4 matrix;        // матрица положения средней точности задающая позицию вершин
attribute lowp vec4 colorAttr;  // // входной аттрибут для координат цветов
varying lowp vec4 color;  // общая переменная для вершин (выходной параметр)

void main () {
    gl_Position = matrix * vertexAttr; // устанавливаем позицию вершин через матрицу
    color = colorAttr; // и передаём цвет в фрагментный шейдер
}
