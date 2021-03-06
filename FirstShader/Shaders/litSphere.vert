varying vec3 vertex_color;
varying vec2 vTexCoord;
varying vec3 lightvec,pos,halfVector;

in vec3 Points;
in vec2 Texcoords;
in vec3 Tangent;
in vec3 Normal;
varying mat3 TBN;

varying vec2 vN;

void main()
{	

	mat3 NormalMatrix = inverse(transpose(gl_ModelViewMatrix));//gl_NormalMatrix;
	vec3 n = normalize( NormalMatrix * Normal );
	//vec3 t = normalize( NormalMatrix * Tangent );
	//vec3 b = ( cross( n, t) );

	//TBN = mat3(t, b, n);

	pos = vec3 (gl_ModelViewMatrix * gl_Vertex);
	
	vec4 p = vec4(pos,1.0);
	vec3 e = normalize( vec3(gl_ModelViewMatrix * p));
	vec3 l = normalize(gl_LightSource[0].position.xyz * p);
	vec3 r = reflect(e,n);
	
	float m = 2 * sqrt( pow(r.x,2) + pow(r.y,2) + pow(r.z+1,2));
	vN = r.xy / m+5;
	gl_Position = gl_ModelViewProjectionMatrix * p;
	
	/*
	vec3 lightDir = normalize(gl_LightSource[0].position.xyz - pos);

	vec3 v;
	v.x = dot (lightDir, t);
	v.y = dot (lightDir, b);
	v.z = dot (lightDir, n);
	lightvec = normalize (v);

	v.x = dot (pos, t);
	v.y = dot (pos, b);
	v.z = dot (pos, n);

	pos = normalize(pos);

	halfVector = normalize (pos + lightDir);
	v.x = dot (halfVector, t);
	v.y = dot (halfVector, b);
	v.z = dot (halfVector, n);
	halfVector = v ; 

	vTexCoord = Texcoords;

	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
*/
	//vertex_color = Tangent;
}

