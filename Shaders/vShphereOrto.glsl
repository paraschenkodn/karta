#version 120
attribute highp vec4 vertexAttr;
uniform mediump mat4 MVPM;
attribute lowp vec4 colorAttr;
//attribute float R; // В картах ATI "левые" переменные для вычислений принимаются только через uniform
uniform float R;
uniform vec4 viewport; //получаем размеры окна рисования (x0,y0,w,h)

// for orto proection
varying float radius;
varying vec3  center;

varying lowp vec4 color;

void main()
{
//gl_Position   = ftransform(); //gl_ModelViewProjectionMatrix * gl_Vertex;   // это не даёт результата без работы с депрекатед матрицами glMatrixMode(GL_PROJECTION); // Матрица проекции
//gl_Position   = vec4(0.5,0.5,0.0,1.0);  // gl_Position - координаты в НОРМАЛИЗОВАНЫХ к 1 координатах экрана с центром в середине
gl_Position = MVPM * vertexAttr;
gl_FrontColor = colorAttr;
color = colorAttr;
gl_PointSize = R * min(viewport.z, viewport.w); //100;  // величина в пикселях экрана

center = gl_Position.xyz;
radius = R;
}
