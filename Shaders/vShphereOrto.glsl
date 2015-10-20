#version 120

void main() {
    gl_Position   = vec4(0.5,0.5,0.0,1.0);
    gl_FrontColor = gl_Color;
    gl_PointSize  = 2;
}
