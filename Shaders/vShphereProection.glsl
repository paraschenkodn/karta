uniform vec4 viewport;          //получаем размеры окна рисования (x0,y0,w,h)
uniform float R;    // "левые" переменные на картах ATI надо передавать через uniform
attribute highp vec4 vertexAttr; //
attribute lowp vec4 colorAttr;  //
uniform mediump mat4 MVPM;  // gl_ModelViewProjectionMatrix
uniform mediump mat4 MVM;  // gl_ModelViewMatrix
uniform mediump mat4 MVPMi; //gl_ModelViewProjectionMatrixInverse
uniform mediump mat4 PMi;  // gl_ProjectionMatrixInverse
uniform mediump mat4 MVMi;  // gl_ModelViewMatrixInverse

varying mat4 VPMTInverse;
varying mat4 VPInverse;
varying vec3 centernormclip;

void main() {
    gl_Position = MVPM * vertexAttr; // В шейдерную программу матрицу нужно передавать как есть, а в шейдере производить умножение не вектора на матрицу, а матрицы на вектор.
    gl_FrontColor = colorAttr; //

    mat4 T = mat4(
            1.0, 0.0, 0.0, 0.0,
            0.0, 1.0, 0.0, 0.0,
            0.0, 0.0, 1.0, 0.0,
            vertexAttr.x/R, vertexAttr.y/R, vertexAttr.z/R, 1.0/R);

    mat4 PMTt = transpose(MVPM * T);  // (Project Model Transpose T)

    vec4 r1 = PMTt[0];
    vec4 r2 = PMTt[1];
    vec4 r4 = PMTt[3];
    float r1Dr4T = dot(r1.xyz,r4.xyz)-r1.w*r4.w;
    float r1Dr1T = dot(r1.xyz,r1.xyz)-r1.w*r1.w;
    float r4Dr4T = dot(r4.xyz,r4.xyz)-r4.w*r4.w;
    float r2Dr2T = dot(r2.xyz,r2.xyz)-r2.w*r2.w;
    float r2Dr4T = dot(r2.xyz,r4.xyz)-r2.w*r4.w;

    gl_Position = vec4(-r1Dr4T, -r2Dr4T, gl_Position.z/gl_Position.w*(-r4Dr4T), -r4Dr4T);


    float discriminant_x = r1Dr4T*r1Dr4T-r4Dr4T*r1Dr1T;
    float discriminant_y = r2Dr4T*r2Dr4T-r4Dr4T*r2Dr2T;
    float screen = max(float(viewport.z), float(viewport.w));

    gl_PointSize = sqrt(max(discriminant_x, discriminant_y))*screen/(-r4Dr4T);
    //gl_PointSize = viewport.w;

    // prepare varyings

    // перенос в локальное пространство координат шара-точки
    mat4 TInverse = mat4(
            1.0,          0.0,          0.0,         0.0,
            0.0,          1.0,          0.0,         0.0,
            0.0,          0.0,          1.0,         0.0,
            -vertexAttr.x, -vertexAttr.y, -vertexAttr.z, R);

    mat4 VInverse = mat4( // TODO: move this one to CPU // лишний код который вычисляется на раз в сцене (не надо его считать для каждой вершины)
            2.0/float(viewport.z), 0.0, 0.0, 0.0,
            0.0, 2.0/float(viewport.w), 0.0, 0.0,
            0.0, 0.0,                   2.0/gl_DepthRange.diff, 0.0,
            -float(viewport.z+2.0*viewport.x)/float(viewport.z), -float(viewport.w+2.0*viewport.y)/float(viewport.w), -(gl_DepthRange.near+gl_DepthRange.far)/gl_DepthRange.diff, 1.0);
    VPInverse = PMi*VInverse; // TODO: move to CPU
    //VPInverse = gl_ProjectionMatrixInverse*VInverse; // TODO: move to CPU

    VPMTInverse = TInverse*MVPMi*VInverse;  // перемножаем с отменой видовых преобразований
    //VPMTInverse = TInverse*gl_ModelViewProjectionMatrixInverse*VInverse;

    vec4 centerclip = MVM * vertexAttr;
    //vec4 centerclip = gl_ModelViewMatrix*vertexAttr;
    centernormclip = vec3(centerclip)/centerclip.w;
}
