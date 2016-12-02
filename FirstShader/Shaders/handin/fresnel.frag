uniform samplerCube CubeMap;

varying float Ratio;

varying vec3 Reflect;
varying vec3 RefractR;
varying vec3 RefractG;
varying vec3 RefractB;

void main()
{

	
	Ratio = 0.3;
	
	vec3 refractColor, reflectColor;

	refractColor.r = vec3 (texture (CubeMap, RefractR)).r;
	refractColor.g = vec3 (texture (CubeMap, RefractG)).g;
	refractColor.b = vec3 (texture (CubeMap, RefractB)).b;

	reflectColor = vec3 (texture (CubeMap, Reflect));

	vec3 color = mix (refractColor, reflectColor, Ratio);

	gl_FragColor = textureCube (CubeMap, color);

	float trans_theta = 0.3 + (1.0 - 0.3) * pow(1.0 - 0.8, 5.0);
	gl_FragColor.rgb = trans_theta * Reflect + (1.0 - trans_theta) * (refractColor * 2); 
	gl_FragColor.a = 0.75; //comment in for translucent effect
}
