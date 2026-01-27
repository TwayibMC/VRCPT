attribute highp vec3 vr_Position;
attribute highp vec2 vr_TexCoord;
uniform   highp mat4 u_ModelViewProjectionMatrix;
uniform   highp float u_Opacity;
uniform   highp vec4 u_Color;
varying   highp vec4 outColor;
varying   highp vec2 outTexCoord;

void main(void)
{
    gl_Position = u_ModelViewProjectionMatrix * vec4(vr_Position,1.0);
    outColor = vec4(u_Color.rgb, u_Opacity);
    outTexCoord = vr_TexCoord;
}
