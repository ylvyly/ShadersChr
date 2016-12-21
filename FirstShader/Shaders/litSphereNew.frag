uniform sampler2D diffuseTexture; //GL_TEXTURE0
uniform sampler2D normalTexture; //GL_TEXTURE1 is actually the attributre tex
uniform sampler2D depthTexture; //GL_TEXTURE2 is actually the normal texture

uniform sampler2D depthBuffer; //GL_TEXTURE3

varying vec2 vTexCoord;
varying vec2 vN;

varying vec3 lightvec, n, lightDir, e, pos, halfVector;

varying mat3 TBN;

varying vec3 t,b;

vec3 erode = vec3(0);
vec3 dilate = vec3(0);
//////////////////////////////////////////////////////////////////////////////////
//erode & dilate function
//////////////////////////////////////////////////////////////////////////////////
void tex2DBiLinear( sampler2D textureSampler_i, vec2 texCoord_i )
{
		vec3 val = vec3(1);
		
		float red = 0.0;
		float green = 0.0;
		float blue = 0.0;

		vec3 px = texture2D(textureSampler_i, texCoord_i).rgb / val;
		vec3 p1 = textureOffset(textureSampler_i, texCoord_i, ivec2(-1, 1)).rgb / val;
		vec3 p2 = textureOffset(textureSampler_i, texCoord_i, ivec2(0, 1)).rgb / val;
		vec3 p3 = textureOffset(textureSampler_i, texCoord_i, ivec2(1 , 1)).rgb / val;
		vec3 p4 = textureOffset(textureSampler_i, texCoord_i, ivec2(-1, 0)).rgb / val;
		vec3 p5 = textureOffset(textureSampler_i, texCoord_i, ivec2(1, 0)).rgb / val;
		vec3 p6 = textureOffset(textureSampler_i, texCoord_i, ivec2(-1, -1)).rgb / val;
		vec3 p7 = textureOffset(textureSampler_i, texCoord_i, ivec2(0, -1)).rgb / val;
		vec3 p8 = textureOffset(textureSampler_i, texCoord_i, ivec2(1, -1)).rgb / val;
	
		red = min(px.x, p5.x);
		red = min(red, p1.x);
		red = min(red, p2.x);
		red = min(red, p3.x);
		red = min(red, p4.x);
		red = min(red, p6.x);
		red = min(red, p7.x);
		red = min(red, p8.x);
		
		green = min(px.y, p5.y);
		green = min(red, p1.y);
		green = min(red, p2.y);
		green = min(red, p3.y);
		green = min(red, p4.y);
		green = min(red, p6.y);
		green = min(red, p7.y);
		green = min(red, p8.y);
		
		blue = min(px.z, p5.z);
		blue = min(red, p1.z);
		blue = min(red, p2.z);
		blue = min(red, p3.z);
		blue = min(red, p4.z);
		blue = min(red, p6.z);
		blue = min(red, p7.z);
		blue = min(red, p8.z);
		
		erode = vec3(red, green, blue);
		
		
		red = max(px.x, p5.x);
		red = max(red, p1.x);
		red = max(red, p2.x);
		red = max(red, p3.x);
		red = max(red, p4.x);
		red = max(red, p6.x);
		red = max(red, p7.x);
		red = max(red, p8.x);
		
		green = max(px.y, p5.y);
		green = max(red, p1.y);
		green = max(red, p2.y);
		green = max(red, p3.y);
		green = max(red, p4.y);
		green = max(red, p6.y);
		green = max(red, p7.y);
		green = max(red, p8.y);
		
		blue = max(px.z, p5.z);
		blue = max(red, p1.z);
		blue = max(red, p2.z);
		blue = max(red, p3.z);
		blue = max(red, p4.z);
		blue = max(red, p6.z);
		blue = max(red, p7.z);
		blue = max(red, p8.z);
		
		dilate = vec3(red, green, blue);
		
		//return erode;
}
//////////////////////////////////////////////////////////////////////////////////


void main()
{

	tex2DBiLinear(diffuseTexture,vN);

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
		newR = smoothstep(0,0.5,brightness) * (r - 1) + 1;
	}
	else {
		newR = (1 - smoothstep(0.5,1,brightness)) * r;
	}
	float m = 2.0 * sqrt( pow(newR.x,2) + pow(newR.y,2) + pow(newR.z,2)); //double length of r
	vec2 newVN = (newR.xy / m) + 0.5;
	base = texture2D(diffuseTexture, newVN).rgba;
	gl_FragColor = base;
	//////////////////////////////////////////////////////////////////////////////////
	
	
	//////////////////////////////////////////////////////////////////////////////////
	//normal mapping
	//////////////////////////////////////////////////////////////////////////////////
	vec3 v;
	v.x = dot (lightDir, t);
	v.y = dot (lightDir, b);
	v.z = dot (lightDir, n);
	vec3 lightvecN = normalize (v);
	v.x = dot (pos, t);
	v.y = dot (pos, b);
	v.z = dot (pos, n);
	pos = normalize(pos);
	halfVector = normalize (pos + lightDir);
	v.x = dot (halfVector, t);
	v.y = dot (halfVector, b);
	v.z = dot (halfVector, n);
	halfVector = v ; 
	vec3 normal = 2.0 * texture (depthTexture, vTexCoord.st).rgb - 1.0;
	normal = normalize (normal * TBN);
	float lamberFactorN = max (dot (lightvecN, normal), 0.0) ;
	float shininessN ;
	vec4 ambientLight = vec4 (0.0,0,0,1);//gl_LightSource[0].ambient;	
	
	if (lamberFactorN > 0.0)
	{		
		shininessN = pow (max (dot (halfVector, normal), 0.0), 2.0)  ;
		gl_FragColor *= lamberFactorN ;
		gl_FragColor +=	vec4(1.0) * shininessN ;		
	}		
	gl_FragColor +=	ambientLight;
	//////////////////////////////////////////////////////////////////////////////////
	
	//////////////////////////////////////////////////////////////////////////////////
	//specular highlight
	//////////////////////////////////////////////////////////////////////////////////
	float lamberFactor = max (dot (lightvec, normal), 0.0) ;
	if (lamberFactor > 0.0)
	{
		//vec3 viewDir = normalize (lightvec);
		vec3 refDir = normalize (- reflect (lightDir,normal));
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

	//////////////////////////////////////////////////////////////////////////////////
	//add filtering
	//////////////////////////////////////////////////////////////////////////////////
	//gl_FragColor.rgb -= erode ;
	
	
	//////////////////////////////////////////////////////////////////////////////////
	
	
	//for testing
	//gl_FragColor += secondary;
	//gl_FragColor = texture(depthBuffer, vTexCoord) ;
	//gl_FragColor = vec4(n, 1.0);
	
	
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
	//gl_FragColor += highlight;
}