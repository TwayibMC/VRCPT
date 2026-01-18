varying highp vec4 outColor;
varying highp vec2 outTexCoord;
uniform sampler2D texture0;
uniform sampler2D texture1;

void main(void)
{
    float c = 0.5;
    vec4 col0 = texture2D(texture0, outTexCoord.ts);
    vec4 col1 = texture2D(texture1, outTexCoord.ts);
    gl_FragColor = mix(col0, col1, c) ;
}
