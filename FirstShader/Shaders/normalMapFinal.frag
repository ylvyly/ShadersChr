varying vec3 vertex_color; 

uniform sampler2D diffuseTexture;
uniform sampler2D normalTexture;

varying vec2 vTexCoord;
varying vec3 lightvec,pos,halfVector;

varying mat3 TBN;

void main()
{

	// lookup normal from normal map, move from [0,1] to  [-1, 1] range, normalize
	vec3 normal = 2.0 * texture (normalTexture, vTexCoord.st).rgb - 1.0;
	normal = normalize (normal * TBN);

	// compute diffuse lighting
	float lamberFactor = max (dot (lightvec, normal), 0.0) ;
	vec4 diffuseMaterial = vec4(0.0);
	vec4 diffuseLight  = vec4(0.0);
	
	// compute specular lighting
	vec4 specularMaterial ;
	vec4 specularLight ;
	float shininess ;
  
	// compute ambient
	vec4 ambientLight = vec4 (0.0,0,0,1);//gl_LightSource[0].ambient;	
	
	if (lamberFactor > 0.0)
	{
		diffuseMaterial = texture(diffuseTexture, vTexCoord);
		diffuseLight  = vec4(1.0);//gl_LightSource[0].diffuse;
		
		specularMaterial =  vec4(1.0)  ;
		specularLight = vec4(1.0);//gl_LightSource[0].specular;
		shininess = pow (max (dot (halfVector, normal), 0.0), 2.0)  ;
		 
		gl_FragColor =	diffuseMaterial* diffuseLight * lamberFactor ;
		gl_FragColor +=	specularMaterial* specularLight* shininess ;				
	
	}		
	gl_FragColor +=	ambientLight;

}