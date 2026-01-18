varying highp vec4 outColor;

void main(void)
{
   vec3 v = outColor.rgb * (1.0 - gl_FragCoord.z);
   gl_FragColor = vec4(v, outColor.a);
}
