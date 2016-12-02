varying vec3 position;

void main()
{	
	position = gl_Vertex.xyz;
	gl_FrontColor = gl_FrontMaterial.diffuse + gl_FrontMaterial.ambient;
	gl_Position = ftransform(); //gl_ModelViewProjectionMatrix * gl_Vertex;
	
}