uniform sampler2D diffuseTexture;
uniform sampler2D normalTexture;

varying vec2 vTexCoord;
varying vec2 vN;

varying vec3 lightvec, n, lightDir,e,pos;

void main()
{
	//////////////////////////////////////////////////////////////////////////////////
	//basic lit sphere shading
	vec4 base = texture2D(diffuseTexture, vN).rgba;
	gl_FragColor = base;
	//////////////////////////////////////////////////////////////////////////////////
	
		
	//////////////////////////////////////////////////////////////////////////////////
	//feature enhancement
	//////////////////////////////////////////////////////////////////////////////////
	vec3 secondary = texture(normalTexture, vTexCoord).rgb ;
	float brightness = (secondary.x + secondary.y + secondary.z) / 3; //average color intensity
	vec3 newR = vec3(0);
	//float oldR = vN.x; 
	vec3 r = reflect(lightDir,n);
	if (brightness < 0.5) {
		newR = smoothstep(0,1,brightness) * (r - 1) + 1;
	}
	else {
		newR = (1 - smoothstep(0,1,brightness)) * r;
	}
	float m = 2.0 * sqrt( pow(newR.x,2) + pow(newR.y,2) + pow(newR.z,2)); //double length of r
	vec2 newVN = (newR.xy / m) + 0.5;
	base = texture2D(diffuseTexture, newVN).rgba;
	gl_FragColor = base;
	//////////////////////////////////////////////////////////////////////////////////
	
	
	//////////////////////////////////////////////////////////////////////////////////
	//specular highlight
	//////////////////////////////////////////////////////////////////////////////////
	float lamberFactor = max (dot (lightvec, n), 0.0) ;
	if (lamberFactor > 0.0)
	{
		//vec3 viewDir = normalize (lightvec);
		vec3 refDir = normalize (- reflect (lightDir,n));
		vec4 shininess = pow (max (dot (refDir,lightvec),0.0),100) ;//pow (max (dot (lightvec, n), 0.0), 100.0) * vec4(1) ;
		//vec4 shininess = max (pow (dot (lightvec,n), 5),0.0) * vec4 (1,1,1,1);
		gl_FragColor +=	shininess ;
	}
	//////////////////////////////////////////////////////////////////////////////////
	
	
	//////////////////////////////////////////////////////////////////////////////////
	//rim lighting
	//////////////////////////////////////////////////////////////////////////////////
	float eta = 0.5;
	float my = 1.1;
	vec3 eye = normalize(-pos);
	float vdn = 1.0 - max(dot(eye, n), 0.0);
	gl_FragColor.rgb += vec3(smoothstep(eta,my,vdn)) ;
	//////////////////////////////////////////////////////////////////////////////////

	
	//gl_FragColor += secondary;
	
	
	//highlight transformation
	//gamma & delta for directional scaling; 1.0 is no scaling
	float gamma = 1.0;
	float delta = 1.0;
	//for rotation
	mat2 rotMatrix;
	float phi = 10; //angle of rotation
	rotMatrix[0] = vec2(cos(-phi), -sin(-phi));
	rotMatrix[1] = vec2(sin(-phi),cos(-phi));
	
	vec2 transfVN = vN * rotMatrix;
	transfVN = vec2( (transfVN.x) / gamma, (transfVN.y ) / delta);
	vec4 highlight = texture2D(diffuseTexture, transfVN).rgba * 0.2;

}