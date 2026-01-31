attribute highp vec3 rv_Position;
attribute highp vec2 rv_TexCoord;
attribute highp vec3 rv_Normal;

uniform   highp mat4 u_ModelMatrix;
uniform   highp mat4 u_ViewMatrix;
uniform   highp mat4 u_ProjectionMatrix;
uniform   highp float u_Opacity;
uniform   highp vec4 u_Color;

varying   highp vec4 outColor;
varying   highp vec2 outTexCoord;
varying   highp vec3 outPos;
varying   highp vec3 outNormal;

void main(void)
{
    outPos = vec3(u_ModelMatrix * vec4(rv_Position, 1.0));
    outNormal = vec3(u_ModelMatrix * vec4(rv_Normal, 0.0));
    outColor = vec4(u_Color.rgb, u_Opacity);
    outTexCoord = rv_TexCoord;

    gl_Position = u_ProjectionMatrix * u_ViewMatrix * vec4(outPos, 1.0);
}
