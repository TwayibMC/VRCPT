attribute highp vec3 vr_Position;
attribute highp vec3 vr_Color;
uniform   highp mat4 u_ModelViewProjectionMatrix;
uniform   highp float u_Opacity;
uniform   highp vec4 u_Color;
varying   highp vec4 outColor;


void main(void)
{
    gl_Position = u_ModelViewProjectionMatrix * vec4(vr_Position, 1.0);
    outColor = vec4(vr_Color * u_Color.rgb, u_Opacity);
}
