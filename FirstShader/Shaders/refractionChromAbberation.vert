//varying vec2 vTexCoord;

float eta = 0.8;
varying vec3 R;  //refracted vector

varying vec3 lightDir, normal, pos;

varying vec3 RedRefractionVector,GreenRefractionVector,BlueRefractionVector;

void main()
{	

	vec4 V = gl_ModelViewMatrix * gl_Vertex;
	vec4 E = gl_ProjectionMatrixInverse * vec4 (0,0,-1,0);
	vec3 I = normalize (V.xyz - E.xyz);
	vec3 N = normalize (gl_Normal);

	R = refract (I, N, eta);

	normal = normalize (gl_NormalMatrix * gl_Normal);
	pos = vec3 (gl_ModelViewMatrix * gl_Vertex);
	lightDir =	normalize (vec3 (gl_LightSource[0].position - pos));
	
	R = refract (pos, normal, 1.9);

	RedRefractionVector = refract (pos, normal, 1.3);
    GreenRefractionVector = refract (pos, normal, 1.9);
    BlueRefractionVector = refract (pos, normal, 1.7);

	//vTexCoord = gl_MultiTexCoord0;
	gl_Position = ftransform(); //gl_ModelViewProjectionMatrix * gl_Vertex;
	
}

