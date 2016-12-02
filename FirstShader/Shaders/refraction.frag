//uniform sampler2D myTexture;

uniform samplerCube CubeMap;
varying vec3 R; //refracted vector

//varying vec2 vTexCoord;
varying vec3 lightDir, normal, pos;
varying vec3 RedRefractionVector,GreenRefractionVector,BlueRefractionVector;

void main()
{

	vec3 viewDir = normalize (-pos);
	vec3 refDir = normalize (- reflect (lightDir,normal));
	vec4 color;

	vec3 n = normalize (normal);

	vec4 i_diffuse = max (dot (lightDir,n), 0.0) * vec4 (0.0,0.0,0.0,1);
	vec4 i_specular = max (pow (dot (refDir,viewDir),100),0.0) * vec4 (1,1,1,1);
	vec4 i_ambient = 1 * vec4 (0,0,0,1);

	//R = normalize(refract(pos, normal, 1.20));
	gl_FragColor = textureCube (CubeMap, R);

	//for chrom. abber.
    //gl_FragColor.r = textureCube(CubeMap, RedRefractionVector).r;
    //gl_FragColor.g = textureCube(CubeMap, GreenRefractionVector).g;
    //gl_FragColor.b = textureCube(CubeMap, BlueRefractionVector).b;


	gl_FragColor.a = 0.6;
}