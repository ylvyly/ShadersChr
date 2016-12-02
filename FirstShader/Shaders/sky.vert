
//varying vec2 texcoords;
void main()
{

	//vTexCoord = gl_MultiTexCoord0;
    gl_TexCoord[0].stp = gl_Vertex.xyz;

	gl_TexCoord[1].stp = gl_Vertex.xyz;
	gl_TexCoord[2].stp = gl_Vertex.xyz;
	gl_TexCoord[3].stp = gl_Vertex.xyz;
	gl_TexCoord[4].stp = gl_Vertex.xyz;
	gl_TexCoord[5].stp = gl_Vertex.xyz;


    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}
