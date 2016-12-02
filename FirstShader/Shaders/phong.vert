const vec4 red = vec4(1.0,0.0,0.0,1.0);

varying vec3 N, v;


void main()
{
	
	v = vec3(gl_ModelViewMatrix * gl_Vertex);
	N = normalize(gl_NormalMatrix * gl_Normal);
	
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	
}