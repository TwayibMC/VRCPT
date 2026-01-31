varying highp vec3 outTexCoord;
uniform samplerCube texture0;

void main(void)
{
    gl_FragColor = textureCube(texture0, outTexCoord);
}
