//varying vec3 vertex_color; 

uniform sampler2D diffuseTexture;

varying vec2 vTexCoord;

void main()
{
	//vec2 tex = vec2(vertex_color.x, -vertex_color.y) * 0.5 + 0.5;
	gl_FragColor = texture2D(diffuseTexture, vTexCoord).rgba;
	//gl_FragColor = vec4(vertex_color,1.0);
}