varying highp vec4 outColor;
varying highp vec2 outTexCoord;
uniform sampler2D texture0;

void main(void)
{
    vec4 texColor = texture2D(texture0, outTexCoord.st);
    gl_FragColor = texColor * outColor;
    gl_FragColor.a = outColor.a;
}
