//uniform sampler2D myTexture;

uniform samplerCube CubeMap;
varying vec3 R; //refracted vector

//varying vec2 vTexCoord;

void main()
{

	//vec4 tex = texture2D(myTexture, vTexCoord).bgra;
	//gl_FragColor = tex;


	gl_FragColor = textureCube (CubeMap, R);
}