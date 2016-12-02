varying vec3 lightDir, normal, pos;

void main() {

	vec3 viewDir = normalize (-pos);
	vec3 refDir = normalize (- reflect (lightDir,normal));
	vec4 color;

	vec3 n = normalize (normal);

	vec4 i_diffuse = max (dot (lightDir,n), 0.0) * vec4 (0.5,0.5,1.0,1);
	vec4 i_specular = max (pow (dot (refDir,viewDir),100),0.0) * vec4 (1,1,1,1);
	vec4 i_ambient = 1 * vec4 (0.2,0,0,1);
	color =  i_diffuse + i_specular + i_ambient ;
	//color.a = 0.75; //transparency

	gl_FragColor = color;

}