varying vec3 vertex_color;

void main()
{	
	gl_Position = ftransform(); //gl_ModelViewProjectionMatrix * gl_Vertex;
	vertex_color = gl_Normal.xyz;
}