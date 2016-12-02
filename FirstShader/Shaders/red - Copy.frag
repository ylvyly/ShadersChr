varying vec4 diffuse, specular,ambient; 
//varying vec3 N;

void main()
{

	gl_FragColor = ambient + diffuse + specular;// * gl_Color ;

}