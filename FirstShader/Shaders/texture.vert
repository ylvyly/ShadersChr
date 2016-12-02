//varying vec3 vertex_color;

varying vec2 vTexCoord;



//attribute vec3 point;
//attribute vec3 normal;
//attribute vec2 texcoord;

//attribute vec3 tangent;

void main()
{	


	vTexCoord = gl_MultiTexCoord0;

	gl_Position = ftransform(); //gl_ModelViewProjectionMatrix * gl_Vertex;

	//vec2 tan = gl_MultiTexCoord1;
	//vertex_color = tangent;//gl_Normal.xyz;
}

