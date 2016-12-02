varying vec3 lightDir, normal, pos;

void main() {

	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;

	normal = normalize (gl_NormalMatrix * gl_Normal);

	pos = vec3 (gl_ModelViewMatrix * gl_Vertex);

	lightDir =	normalize (vec3 (gl_LightSource[0].position - pos));
	

}