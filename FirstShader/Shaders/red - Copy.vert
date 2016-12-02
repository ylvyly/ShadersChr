varying vec4 diffuse, specular,ambient;
varying vec3 N, normal;

void main()
{	
	gl_Position = ftransform(); //gl_ModelViewProjectionMatrix * gl_Vertex;
	normal = gl_Normal;

	vec3 lightVector = gl_LightSource[0].position.xyz - gl_Vertex.xyz;
	
	N = normalize(gl_NormalMatrix * normal);
	vec3 E = normalize(normal); 
	vec3 R = normalize(-reflect(lightVector,N));  
	
	float dist = length(lightVector);

	float attenuation = 1.0 / (gl_LightSource[0].constantAttenuation +  
								gl_LightSource[0].linearAttenuation  * dist + 
								gl_LightSource[0].quadraticAttenuation * dist * dist); 

	lightVector = normalize(lightVector);
	
	float nxDir = max(0.0, dot(normal,lightVector));

	ambient = gl_FrontLightProduct[0].ambient * gl_FrontMaterial.ambient;
	
	diffuse = gl_LightSource[0].diffuse * max(dot(N,lightVector),0.0) * gl_FrontMaterial.diffuse;
	diffuse = clamp(diffuse, 0.0, 1.0); 

	// calculate Specular Term:
	//gl_FrontMaterial.shininess *= 5.1; 
	specular = gl_FrontLightProduct[0].specular * pow(max(dot(R,E),0.0), gl_FrontMaterial.shininess) * attenuation;
	specular = clamp(specular, 0.0, 1.0); 

	
	//diffuse = (gl_FrontLightProduct[0].diffuse * max(dot(N,lightVector),0.0)) * nxDir;
	//diffuse = diffuse * nxDir * attenuation;
	
}