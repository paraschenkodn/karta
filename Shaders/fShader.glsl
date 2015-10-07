varying lowp vec4 color;  // входная переменная (переданная из вершинного шейдера)

void main () {
    gl_FragColor = color; // просто принимаем значение без обработки
}
