//varying vec4 color_out;
varying vec3 N, v;


void main()
{
	vec3 L = normalize(gl_LightSource[0].position.xyz-v);
	vec3 E = normalize(-v);
	vec3 R = normalize(-reflect(L,N));

	vec4 Idiff = gl_FrontLightProduct[0].diffuse* max(dot(N,L),0.0);
	Idiff = clamp(Idiff,0.0,1.0);

	vec4 IAmb = gl_FrontLightProduct[0].ambient * gl_FrontMaterial.ambient;

	vec4 ISpec = gl_FrontLightProduct[0].specular * pow(max(dot(R,E),0.0),gl_FrontMaterial.shininess);
	ISpec = clamp(ISpec,0.0,1.0);

	gl_FragColor = Idiff + ISpec + IAmb;
}


