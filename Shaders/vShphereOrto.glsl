#version 120

uniform vec4 viewport;
attribute float R;

varying float radius;
varying vec2  center;

void main() {
    gl_Position   = gl_ModelViewProjectionMatrix * gl_Vertex;
    gl_FrontColor = gl_Color;
    gl_PointSize  = R * min(viewport.z, viewport.w);

    center = gl_Position.xy;
    radius = R;
}
