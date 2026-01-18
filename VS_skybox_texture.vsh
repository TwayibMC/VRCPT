attribute highp vec3 rv_Position;
uniform   highp mat4 u_ModelViewProjectionMatrix;
varying   highp vec3 outTexCoord;


void main(void)
{
    gl_Position = u_ModelViewProjectionMatrix * vec4(rv_Position, 1.0);
    outTexCoord = rv_Position;
}
