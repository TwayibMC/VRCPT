varying highp vec4 outColor;
varying highp vec2 outTexCoord;
uniform sampler2D texture0;

void main(void)
{
    float t = 1.0 - gl_FragCoord.z;
    t = smoothstep(0.2, 0.8, t);
    gl_FragColor = t*texture2D(texture0, outTexCoord);
    gl_FragColor.a = outColor.a;
}
