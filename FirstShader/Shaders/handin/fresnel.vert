
const float EtaR = 0.65;
const float EtaG = 0.67; //ratio of indices of refraction
const float EtaB = 0.69;

const float F = ((1 - EtaG) * (1 - EtaG)) / ((1 + EtaG) * (1 + EtaG));

varying vec3 Reflect;
varying vec3 RefractR, RefractG, RefractB;

varying float Ratio;

varying vec3 lightDir, normal, pos;


void main()
{	

	normal = normalize (gl_NormalMatrix * gl_Normal);
	pos = vec3 (gl_ModelViewMatrix * gl_Vertex);
	lightDir =	normalize (vec3 (gl_LightSource[0].position - pos));

	vec4 V = gl_ModelViewMatrix * gl_Vertex;
	vec4 E = gl_ProjectionMatrixInverse * vec4 (0,0,-1,0);
	vec3 I = normalize (V.xyz - E.xyz);
	vec3 N = normalize (gl_Normal);

	Ratio = F + (1.0 - F) * pow ((1.0 - dot(-I,N)), 5); //Schlick

	RefractR = refract (I, N, EtaR);
	RefractR = refract (pos, normal, 1.6);

	RefractG = refract (I, N, EtaG);
	RefractG = refract (pos, normal, 1.7);

	RefractB = refract (I, N, EtaB);
	RefractB = refract (pos, normal, 1.65);

	Reflect = reflect (I, N);

	gl_Position = ftransform(); //gl_ModelViewProjectionMatrix * gl_Vertex;
	
}

