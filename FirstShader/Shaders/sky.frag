//uniform samplerCube Skybox;
//varying vec2 texcoords;
uniform sampler2D Skybox0;
uniform sampler2D Skybox1;
uniform sampler2D Skybox2;
uniform sampler2D Skybox3;
uniform sampler2D Skybox4;
uniform sampler2D Skybox5;

uniform int usetexture  = 0;

void main()
{
	vec4 color;
 
	if (usetexture == 0)
		color = texture(Skybox0, gl_TexCoord[0].stp);//texcoords);
	if (usetexture == 1)
		color = texture(Skybox1, gl_TexCoord[1].stp);//texcoords);
	if (usetexture == 2)
		color = texture(Skybox2, gl_TexCoord[2].stp);//texcoords);
	if (usetexture == 3)
		color = texture(Skybox3, gl_TexCoord[3].stp);//texcoords);
	if (usetexture == 4)
		color = texture(Skybox4, gl_TexCoord[4].stp);//texcoords);
	if (usetexture == 5)
		color = texture(Skybox5, gl_TexCoord[5].stp);//texcoords);
 
	gl_FragColor = color;
	

    //gl_FragColor = textureCube(Skybox, gl_TexCoord[0].stp);
}