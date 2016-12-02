varying vec3 Reflect;

void main()
{	

	vec4 V = gl_ModelViewMatrix * gl_Vertex;
	vec4 E = gl_ProjectionMatrixInverse * vec4 (0,0,-1,0);
	vec3 I = normalize (V.xyz - E.xyz);	
	vec3 N = normalize (gl_NormalMatrix * gl_Normal);
	
	Reflect = reflect (I, N);

	gl_Position = ftransform(); //gl_ModelViewProjectionMatrix * gl_Vertex;
	
}