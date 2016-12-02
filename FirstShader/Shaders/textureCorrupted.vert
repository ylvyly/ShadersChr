varying vec3 vertex_color;

varying vec2 vTexCoord;
//varying vec3 eyeVec;

varying vec3 lightvec,pos;

in vec3 Points;
in vec2 Texcoords;
in vec3 Tangent;
in vec3 Normal;

void main()
{	

	vec3 n = normalize( gl_NormalMatrix * Normal );
	vec3 t = normalize( gl_NormalMatrix * Tangent );
	vec3 b = ( cross( n, t) );

	mat3 TBN = mat3(t, b, n);

	pos = vec3 (gl_ModelViewMatrix * gl_Vertex);
	vec3 lightDir = normalize(gl_LightSource[0].position.xyz - pos);

	vec3 v;
	v.x = dot (lightDir, t);
	v.y = dot (lightDir, b);
	v.z = dot (lightDir, n);
	lightvec = normalize (v);

	//lightvec *= TBN;

	v.x = dot (pos, t);
	v.y = dot (pos, b);
	v.z = dot (pos, n);
	//eyeVec = normalize (v);

	pos = normalize(pos);

	vec3 halfVector = normalize (pos + lightDir);
	v.x = dot (halfVector, t);
	v.y = dot (halfVector, b);
	v.z = dot (halfVector, n);
	halfVector = v ; 

	vTexCoord = Texcoords;
	//vTexCoord = vec2(gl_MultiTexCoord0.x, 1.0 - gl_MultiTexCoord0.y);

	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;

	//vec2 tan = gl_MultiTexCoord1;
	vertex_color = Tangent; //vec3(Texcoords,1.0);// vec3(tan,0.5);//gl_Normal.xyz;
}

