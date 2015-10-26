#version 120

varying lowp vec4 color;  // входная переменная (переданная из вершинного шейдера)

uniform vec4 viewport2;
varying float radius;
varying vec2  center;

void main(void) {
    //gl_FragColor = color; // просто принимаем значение без обработки
    //gl_FragColor = vec4(1.0, 0.0, 1.0, 1.0);
    vec2 ndc_current_pixel = ((2.0 * gl_FragCoord.xy) - (2.0 * viewport2.xy)) / (viewport2.zw) - 1;

    vec2 diff = ndc_current_pixel - center;  // вычисляем разницу векторов между точкой и центром
    float d2 = dot(diff,diff);              // ??скалярное произведение разницы на разницу??
    float r2 = radius*radius;               // радиус на радиус, теперь можем сравнить два скалярных произведения

    if (d2>r2) {        // если скаляр радиуса меньше скаляра вектора от центра сферы до точки поверхности, отбрасываем, иначе рисуем сферу
        discard;
    } else {
        vec3 l = normalize(gl_LightSource[0].position.xyz);
        float dr =  sqrt(r2-d2);
        vec3 n = vec3(ndc_current_pixel-center, dr);
        float intensity = .2 + max(dot(l,normalize(n)), 0.0);
        gl_FragColor = gl_Color*intensity;
        gl_FragDepth = gl_FragCoord.z + dr*gl_DepthRange.diff/2.0*gl_ProjectionMatrix[2].z;
    }
}

//Модель диффузного освещения

//Эта модель дает очень неплохой результат, так как интенсивность освещения каждой точки модели рассчитывается в зависимости от направления нормали поверхности и позиции источника света. В результате мы получаем неплохое затенение объекта. Общая формула этой модели выглядит так:

//Idiffuse=Kd×Id×dot(N, L)
// Idiffuse - степень освещенности для каждого пикселя
//Kd - цвет материала
//Id - интенсивность освещения
//N - вектор нормали поверхности
//L - положение источника освещения
//dot - скалярное произведение (dot product) двух векторов.

