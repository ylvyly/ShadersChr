
const float EtaR = 0.65;
const float EtaG = 0.67; //ratio of indices of refraction
const float EtaB = 0.69;

const float F = ((1 - EtaG) * (1 - EtaG)) / ((1 + EtaG) * (1 + EtaG));

varying vec3 Reflect;
varying vec3 RefractR, RefractG, RefractB;

varying float Ratio;

//uniform mat4 TextureMatrix;
//gl_MultiTexCoord0

varying vec3 lightDir, normal, pos;


void main()
{	

	normal = normalize (gl_NormalMatrix * gl_Normal);
	pos = vec3 (gl_ModelViewMatrix * gl_Vertex);
	lightDir =	normalize (vec3 (gl_LightSource[0].position - pos));
	//vec4 TextureMatrix = gl_MultiTexCoord0;

	vec4 V = gl_ModelViewMatrix * gl_Vertex;
	vec4 E = gl_ProjectionMatrixInverse * vec4 (0,0,-1,0);
	vec3 I = normalize (V.xyz - E.xyz);
	vec3 N = normalize (gl_Normal);

	Ratio = F + (1.0 - F) * pow ((1.0 - dot(-I,N)), 5); //Schlick

	RefractR = refract (I, N, EtaR);
	RefractR = refract (pos, normal, 1.6);
	//RefractR = vec3 (TextureMatrix * vec4 (RefractR, 1.0));

	RefractG = refract (I, N, EtaG);
	RefractG = refract (pos, normal, 1.7);
	//RefractG = vec3 (TextureMatrix * vec4 (RefractG, 1.0));

	RefractB = refract (I, N, EtaB);
	RefractB = refract (pos, normal, 1.65);
	//RefractB = vec3 (TextureMatrix * vec4 (RefractB, 1.0));

	Reflect = reflect (I, N);
	//Reflect = vec3 (TextureMatrix * vec4 (Reflect, 1.0));

	gl_Position = ftransform(); //gl_ModelViewProjectionMatrix * gl_Vertex;
	
}






/*
uniform mat4 MVMatrix = gl_ModelViewMatrix;
uniform mat4 MVPMatrix = gl_ModelViewProjectionMatrix;

uniform mat4 TextureMatrix;
uniform mat3 NormalMatrix = gl_NormalMatrix;

in vec4 MCvertex;
in vec3 MCnormal;

const float EtaR = 0.65;
const float EtaG = 0.67; //ratio of indices of refraction
const float EtaB = 0.69;

const float F = ((1 - EtaG) * (1 - EtaG)) / ((1 + EtaG) * (1 + EtaG));

out vec3 Reflect;
out vec3 RefractR, RefractG, RefractB;

out float Ratio;

float eta = 0.8;

void main() {

	vec4 V = gl_ModelViewMatrix * gl_Vertex;
	vec4 E = gl_ProjectionMatrixInverse * vec4 (0,0,-1,0);
	vec3 I = normalize (V.xyz - E.xyz);
	vec3 N = normalize (gl_Normal);

	//R = refract (I, N, eta);

	//vec4 ecPosition = MVMatrix * MCvertex;
	//vec4 ecPosition = gl_ModelViewMatrix * MCvertex;
	//vec3 ecPosition3 = ecPosition.xyz / ecPosition.w;

	//vec3 i = normalize (ecPosition3);
	//vec4 n = normalize(NormalMatrix * MCnormal);

	Ratio = F + (1.0 - F) * pow((1.0 - dot(-i,n)), 5); //Schlick

	RefractR = refract (I, N, EtaR);
	RefractR = vec3 (TextureMatrix * vec4 (RefractR, 1.0));

	RefractG = refract (I, N, EtaG);
	RefractG = vec3 (TextureMatrix * vec4 (RefractG, 1.0));

	RefractB = refract (I, N, EtaB);
	RefractB = vec3 (TextureMatrix * vec4 (RefractB, 1.0));

	Reflect = reflect (I, N);
	Reflect = vec3 (TextureMatrix * vec4 (Reflect, 1.0));

	gl_Position = MVPMatrix * gl_Vertex; //* MCvertex;
	//gl_Position = ftransform();

	}*/