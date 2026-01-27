attribute highp vec3 vr_Position;
uniform   highp mat4 u_ModelViewProjectionMatrix;
varying   highp vec3 outTexCoord;


void main(void)
{
    gl_Position = u_ModelViewProjectionMatrix * vec4(vr_Position, 1.0);
    outTexCoord = vr_Position;
}
