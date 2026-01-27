attribute highp vec3 vr_Position;
attribute highp vec3 vr_Color;
attribute highp vec3 vr_TexCoord;
attribute highp vec3 vr_Normal;

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
    outPos = vec3(u_ModelMatrix * vec4(vr_Position, 1.0));
    outNormal = vec3(u_ModelMatrix * vec4(vr_Normal, 0.0));
    //outColor = vec4(u_Color.rgb * vr_Color, u_Opacity);
    outColor = vec4(u_Color.rgb, u_Opacity);
    outTexCoord = vr_TexCoord.st;

    gl_Position = u_ProjectionMatrix * u_ViewMatrix * vec4(outPos, 1.0);
}
