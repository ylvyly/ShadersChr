//varying vec2 vTexCoord;

float eta = 0.8;
varying vec3 R;  //refracted vector


void main()
{	

	vec4 V = gl_ModelViewMatrix * gl_Vertex;
	vec4 E = gl_ProjectionMatrixInverse * vec4 (0,0,-1,0);
	vec3 I = normalize (V.xyz - E.xyz);
	vec3 N = normalize (gl_Normal);

	R = refract (I, N, eta);
	
	//vTexCoord = gl_MultiTexCoord0;
	gl_Position = ftransform(); //gl_ModelViewProjectionMatrix * gl_Vertex;
	
}

