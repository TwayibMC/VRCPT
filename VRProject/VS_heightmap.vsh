attribute highp vec3 vr_Position;
attribute highp vec2 vr_Color;
attribute highp vec3 vr_TexCoord;
attribute highp vec3 vr_Normal;

uniform highp mat4  u_ModelMatrix;
uniform highp mat4  u_ViewMatrix;
uniform highp mat4  u_ProjectionMatrix;
uniform highp float u_Opacity;
uniform highp vec4  u_Color;

uniform highp sampler2D u_heightmap;
uniform highp float     u_facteur;

varying highp vec4 outColor;
varying highp vec2 outTexCoord;
varying highp vec3 outPos;
varying highp vec3 outNormal;

void main(void)
{
    vec3 pos = vr_Position;

    // Heightmap (unit 1 côté C++)
    float h = texture2DLod(u_heightmap, vr_TexCoord.st, 0.0).r;
    pos.y += u_facteur * h;

    outPos     = vec3(u_ModelMatrix * vec4(pos, 1.0));
    outNormal  = vec3(u_ModelMatrix * vec4(vr_Normal, 0.0));
    outColor   = vec4(u_Color.rgb, u_Opacity);
    outTexCoord = vr_TexCoord.st;

    gl_Position = u_ProjectionMatrix * u_ViewMatrix * vec4(outPos, 1.0);
}
