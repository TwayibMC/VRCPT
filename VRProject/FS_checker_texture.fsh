varying highp vec4 outColor;
varying highp vec2 outTexCoord;
uniform sampler2D texture0;

void main(void)
{
    vec4 col1 = outColor;
    vec4 col2 = vec4(0.0, 0.0, 0.0, 1.0);

    float res = mod(floor(10.0*outTexCoord.s)+floor(26.0*outTexCoord.t), 2.0);
    gl_FragColor = (col1*res + col2*(1.0 - res));
}

