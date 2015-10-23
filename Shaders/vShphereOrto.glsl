attribute highp vec4 vertexAttr;
uniform mediump mat4 viewport;
attribute lowp vec4 colorAttr;
attribute float R;
uniform vec4 viewport2;
//out int pointsize; // output for debug

// for orto proection
varying float radius;
varying vec2  center;

// for perspective proection
varying mat4 VPMTInverse;
varying mat4 VPInverse;
varying vec3 centernormclip;

varying lowp vec4 color;

void main()
{
//gl_Position   = ftransform(); //gl_ModelViewProjectionMatrix * gl_Vertex;   // это не даёт результата без работы с нативными матрицами glMatrixMode(GL_PROJECTION); // Матрица проекции
//gl_Position   = vec4(0.5,0.5,0.0,1.0);  // координаты в координатах сцены
gl_Position = viewport * vertexAttr;
gl_FrontColor = colorAttr;
color = colorAttr;
gl_PointSize = R * min(viewport2.z, viewport2.w); //100;  // величина в пикселях экрана

center = gl_Position.xy;
radius = R;
}

