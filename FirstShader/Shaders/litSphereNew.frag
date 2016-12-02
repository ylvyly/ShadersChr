uniform sampler2D diffuseTexture;
uniform sampler2D normalTexture;

varying vec2 vTexCoord;
varying vec2 vN;

void main()
{
	
	vec4 base = texture2D(diffuseTexture, vN).rgba;
	
	vec3 secondary = 2.0 * texture(normalTexture, vN).rgb - 1.0;
	
	
	
	gl_FragColor = base;// + (secondary *0.0);
	//gl_FragColor = texture2D(diffuseTexture, vTexCoord).rgba;

}