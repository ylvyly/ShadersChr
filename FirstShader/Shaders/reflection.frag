uniform samplerCube CubeMap;

varying vec3 Reflect;

void main()
{

	//vec3 reflectColor;

	//reflectColor = vec3 (texture (CubeMap, Reflect));

	//vec3 color =  reflectColor;

	gl_FragColor = textureCube (CubeMap, Reflect);
	//gl_FragColor.a = 0.75; //comment in for translucent effect
}
