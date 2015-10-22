attribute highp vec4 vertexAttr;
uniform mediump mat4 viewport;
attribute lowp vec4 colorAttr;
attribute float R;
uniform vec4 viewport2;
//out int pointsize; // output for debug

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

    mat4 T = mat4(
            1.0, 0.0, 0.0, 0.0,
            0.0, 1.0, 0.0, 0.0,
            0.0, 0.0, 1.0, 0.0,
            gl_Vertex.x/R, gl_Vertex.y/R, gl_Vertex.z/R, 1.0/R);

    mat4 PMTt = transpose(gl_ModelViewProjectionMatrix * T);

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
    float screen = max(float(viewport2.z), float(viewport2.w));

    gl_PointSize = sqrt(max(discriminant_x, discriminant_y))*screen/(-r4Dr4T);


    // prepare varyings

    mat4 TInverse = mat4(
            1.0,          0.0,          0.0,         0.0,
            0.0,          1.0,          0.0,         0.0,
            0.0,          0.0,          1.0,         0.0,
            -gl_Vertex.x, -gl_Vertex.y, -gl_Vertex.z, R);
    mat4 VInverse = mat4( // TODO: move this one to CPU
            2.0/float(viewport2.z), 0.0, 0.0, 0.0,
            0.0, 2.0/float(viewport2.w), 0.0, 0.0,
            0.0, 0.0,                   2.0/gl_DepthRange.diff, 0.0,
            -float(viewport2.z+2.0*viewport2.x)/float(viewport2.z), -float(viewport2.w+2.0*viewport2.y)/float(viewport2.w), -(gl_DepthRange.near+gl_DepthRange.far)/gl_DepthRange.diff, 1.0);
    VPMTInverse = TInverse*gl_ModelViewProjectionMatrixInverse*VInverse;
    VPInverse = gl_ProjectionMatrixInverse*VInverse; // TODO: move to CPU
    vec4 centerclip = gl_ModelViewMatrix*gl_Vertex;
    centernormclip = vec3(centerclip)/centerclip.w;
}
