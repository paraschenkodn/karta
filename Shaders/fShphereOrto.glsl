#version 120
uniform vec4 viewport; //получаем размеры окна рисования (x0,y0,w,h)
varying float radius;
varying vec3  center;

void main(void) {
    //gl_FragColor = color; // просто принимаем значение без обработки
    //gl_FragColor = vec4(1.0, 0.0, 1.0, 1.0);

    vec2 ndc_current_pixel = ((2.0 * gl_FragCoord.xy) - (2.0 * viewport.xy)) / (viewport.zw) - 1; //
    //(позиция gl_Position (0 в центре экрана, координаты сцены) соотносится к gl_FragCoord (0 внизу слева, координаты экрана)
    // как gl_FragCoord = gl_Position * 0,5 + 0,5 ) (2d_coord_vertex = viewport.xy + viewport.wh * (1 + gl_Position.xy / gl_Position.w)/2)

    vec2 diff = ndc_current_pixel - center.xy;  // вычисляем разницу векторов между точкой и центром
    float d2 = dot(diff,diff);                  // ??скалярное произведение разницы на разницу??
    float r2 = radius*radius;               // радиус на радиус, теперь можем сравнить два скалярных произведения

    if (d2>r2) {        // если скаляр радиуса меньше скаляра вектора от центра сферы до точки поверхности, отбрасываем, иначе рисуем сферу
        //discard;
    } else {
        vec3 l = normalize(gl_LightSource[0].position.xyz);
        float dr =  sqrt(r2-d2);
        vec3 n = vec3(ndc_current_pixel-center.xy, dr);
        float intensity = .2 + max(dot(l,normalize(n)), 0.0);
        gl_FragColor = gl_Color*intensity;
        gl_FragDepth =  gl_FragCoord.z - dr*gl_DepthRange.diff/2.0*gl_ProjectionMatrix[2].z;
    }
    //gl_FragColor = vec4(1.0, 0.0, 1.0, 1.0);
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

