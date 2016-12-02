uniform samplerCube CubeMap;

varying vec3 Reflect;

void main()
{

	gl_FragColor = textureCube (CubeMap, Reflect);
	//gl_FragColor.a = 0.75; //comment in for translucent effect
}
