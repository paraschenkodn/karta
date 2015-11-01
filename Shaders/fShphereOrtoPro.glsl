#version 120
varying mat4 VPMTInverse;
varying mat4 VPInverse;
varying vec3 centernormclip;

// временный костыль
uniform vec4 viewport;
varying lowp vec4 color;
varying float radius;
varying vec3  center;
varying vec4  position;
// енд костыль

void main(void) {

    // временный костыль
    //vec2 ndc_current_pixel = (((2.0 * gl_FragCoord.xy) - (2.0 * viewport.xy)) / (viewport.zw)) - 1; //
    //ndc_current_pixel=ndc_current_pixel/gl_FragCoord.w;
    //vec2 ndc_current_pixel = (((gl_FragCoord.xy - viewport.xy) / (viewport.zw)) - 1)*2*gl_FragCoord.w;
    //vec2 ndc_current_pixel = (2.0 * (gl_FragCoord.xy / viewport.zw)) - 1; //
    //ndc_current_pixel=ndc_current_pixel/3 + center.xy;

    //vec3 tmp_center = position.xyz / position.w;
    //vec2 diff = gl_FragCoord.xy - tmp_center.xy;  // вычисляем разницу векторов между точкой и центром (center - в координатах сцены?? gl_FragCoord - в координатах экрана??)

    vec4 ndcPos;
    ndcPos.xy = ((gl_FragCoord.xy / viewport.zw) * 2.0) - 1.0;
    ndcPos.z = (2.0 * gl_FragCoord.z - gl_DepthRange.near - gl_DepthRange.far) / (gl_DepthRange.far - gl_DepthRange.near);
    ndcPos.w = 1.0;
    vec4 ndc_current_pixel = ndcPos / gl_FragCoord.w  ;
    float tmp_radius=radius;

    vec2 diff = ndc_current_pixel.xy - center.xy;  // вычисляем разницу векторов между точкой и центром
    float d2 = dot(diff,diff);              // ??скалярное произведение разницы на разницу??
    float r2 = tmp_radius*tmp_radius;               // радиус на радиус, теперь можем сравнить два скалярных произведения

    //gl_FragColor = vec4(1.0, 0.0, 1.0, 1.0);
    if (d2>r2) {        // если скаляр радиуса меньше скаляра вектора от центра сферы до точки поверхности, отбрасываем, иначе рисуем сферу
        discard;
    } else {
        vec3 l = normalize(gl_LightSource[0].position.xyz);
        float dr =  sqrt(r2-d2);
        vec3 n = vec3((ndc_current_pixel.xy - center.xy), dr);
        float intensity = .2 + max(dot(l,normalize(n)), 0.0);
        gl_FragColor = gl_Color*intensity;
        gl_FragDepth =  gl_FragCoord.z - dr*gl_DepthRange.diff/2.0*gl_ProjectionMatrix[2].z;
        //gl_FragColor = vec4(1.0, 0.0, 1.0, 1.0);
    }

    //gl_FragColor = gl_Color;
}
