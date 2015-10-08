varying lowp vec4 color;  // входная переменная (переданная из вершинного шейдера)
varying highp vec2 texCoord;  // входная переменная (переданная из вершинного шейдера)
uniform sampler2D texUniform;

void main () {
    gl_FragColor = color; // просто принимаем значение без обработки
    //gl_FragColor = texture2D(texUniform, texCoord); //
}
