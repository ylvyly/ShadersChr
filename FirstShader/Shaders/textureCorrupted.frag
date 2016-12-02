varying vec3 vertex_color; 

uniform sampler2D diffuseTexture;
uniform sampler2D normalTexture;

varying vec2 vTexCoord;

varying vec3 lightvec,pos;

void main()
{

	// lookup normal from normal map, move from [0,1] to  [-1, 1] range, normalize
	vec3 normal = 2.0 * texture (normalTexture, vTexCoord.st).rgb - 1.0;
	normal = normalize (normal);
	
	// compute diffuse lighting
	float lamberFactor= 0.0;//max (dot (lightVec, normal), 0.0) ;
	vec4 diffuseMaterial = 0.0;
	vec4 diffuseLight  = 0.0;
	
	// compute specular lighting
	vec4 specularMaterial ;
	vec4 specularLight ;
	float shininess ;
  
	// compute ambient
	vec4 ambientLight = gl_LightSource[0].ambient;	
	
	if (lamberFactor > 0.0)
	{
		diffuseMaterial = texture(diffuseTexture, vTexCoord.st);
		diffuseLight  = gl_LightSource[0].diffuse;
		
		specularMaterial =  vec4(1.0)  ;
		specularLight = gl_LightSource[0].specular;
		shininess = pow (max (dot (halfVec, normal), 0.0), 2.0)  ;
		 
		gl_FragColor =	diffuseMaterial * diffuseLight * lamberFactor ;
		gl_FragColor +=	specularMaterial * specularLight * shininess ;				
	
	}
	
	gl_FragColor +=	ambientLight;

	gl_FragColor =  vec4(vertex_color,1.0);

	//gl_FragColor = diffuseMaterial;
	
/*
	vec3 normal = 2.0 * texture(normalTexture, vTexCoord).rgb - 1.0;

	//vec2 tex = vec2(vertex_color.x, -vertex_color.y) * 0.5 + 0.5;
	//gl_FragColor = texture2D(normalTexture, vTexCoord).bgra;
	//gl_FragColor = mix(texture(diffuseTexture, vTexCoord), texture(normalTexture, vTexCoord), 0.2);
	//gl_FragColor = vec4(vertex_color,1.0);

	
	vec3 norm = 2.0 * texture (normalTexture, vTexCoord).rgb - 1.0;
	vec3 baseColor = texture(diffuseTexture, vTexCoord).rgb;
	
	float dist = length(lightvec);
	
	vec3 lightVector = normalize(lightvec);
	float nxDir = max(0.0, dot(norm, lightVector));
	//vec4 diffuse = gl_LightSource[0].diffuse * nxDir;
	
	float specularPower = 0.0;
	if(nxDir != 0.0)
	{
	
		vec3 cameraVector = normalize(pos);
		vec3 halfVector = normalize(lightVector + cameraVector);
		float nxHalf = max(0.0, dot(norm, halfVector));
		specularPower = pow(nxHalf, gl_FrontMaterial.shininess);
		
	}
	
	vec4 specular = gl_LightSource[0].specular * specularPower;
	vec4 textureColor = texture(diffuseTexture, vTexCoord).bgra;
	gl_FragColor =   gl_LightSource[0].ambient + textureColor + gl_FrontLightModelProduct.sceneColor +	specular;//gl_LightSource[0].ambient + baseColor + gl_FrontLightModelProduct.sceneColor +	specular; //+ color;
	

	//gl_FragColor =  vec4(vertex_color,1.0);

	*/
}