//uniform sampler2D myTexture;

uniform samplerCube CubeMap;
varying vec3 R; //refracted vector

//varying vec2 vTexCoord;

void main()
{

	gl_FragColor = textureCube (CubeMap, R);

}