varying highp vec4 outColor;
varying highp vec2 outTexCoord;
uniform sampler2D texture0;

void main(void)
{
    gl_FragColor = texture2D(texture0, outTexCoord.ts);
}
