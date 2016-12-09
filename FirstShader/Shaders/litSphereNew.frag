uniform sampler2D diffuseTexture;
uniform sampler2D normalTexture;

varying vec2 vTexCoord;
varying vec2 vN;

varying vec3 lightvec, n, lightDir,e,pos;

vec3 erode = vec3(0);
vec3 dilate = vec3(0);

//erode & dilate
void tex2DBiLinear( sampler2D textureSampler_i, vec2 texCoord_i )
{
		vec3 val = vec3(1);
		
		float red = 0.0;
		float green = 0.0;
		float blue = 0.0;

		vec3 p0q0 = texture2D(textureSampler_i, texCoord_i).rgb / val;
		vec3 p1q0 = textureOffset(textureSampler_i, texCoord_i, ivec2(1, 0)).rgb / val;
		vec3 p0q1 = textureOffset(textureSampler_i, texCoord_i, ivec2(0, 1)).rgb / val;
		vec3 p1q1 = textureOffset(textureSampler_i, texCoord_i, ivec2(1 , 1)).rgb / val;

		//vec4 pInterp_q0 = mix( p0q0, p1q0, 0.5 ); // Interpolates top row in X direction.
		//vec4 pInterp_q1 = mix( p0q1, p1q1, 0.5 ); // Interpolates bottom row in X direction.
		//return mix( pInterp_q0, pInterp_q1, 0.5 ); // Interpolate in Y direction.
		
		red = min(p0q0.x, p1q0.x);
		red = min(red, p0q1.x);
		red = min(red, p1q1.x);
		
		green = min(p0q0.y, p1q0.y);
		green = min(red, p0q1.y);
		green = min(red, p1q1.y);
		
		blue = min(p0q0.z, p1q0.z);
		blue = min(red, p0q1.z);
		blue = min(red, p1q1.z);
		
		erode = vec3(red, green, blue);
		
		red = max(p0q0.x, p1q0.x);
		red = max(red, p0q1.x);
		red = max(red, p1q1.x);
		
		green = max(p0q0.y, p1q0.y);
		green = max(red, p0q1.y);
		green = max(red, p1q1.y);
		
		blue = max(p0q0.z, p1q0.z);
		blue = max(red, p0q1.z);
		blue = max(red, p1q1.z);
		
		dilate = vec3(red, green, blue);
		
		//return erode;
}

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

	/*
	//for low frequency decomposition
	// 1. erode
	int i = vN.x;
	int j = vN.y;

			//for red
			float minR = texture2D(diffuseTexture, vec2(i-1,j-1)).r;
			minR = minR / dot(n, );
			if (texture2D(diffuseTexture, vec2(i,j-1)).r) {
				minR = texture2D(diffuseTexture, vec2(i,j-1)).r;
			}
			if (texture2D(diffuseTexture, vec2(i+1,j-1)).r) {
				minR = texture2D(diffuseTexture, vec2(i+1,j-1)).r;
			}
			if (texture2D(diffuseTexture, vec2(i-1,j)).r) {
				minR = texture2D(diffuseTexture, vec2(i-1,j)).r;
			}
			if (texture2D(diffuseTexture, vec2(i+1,j)).r) {
				minR = texture2D(diffuseTexture, vec2(i+1,j)).r;
			}
			if (texture2D(diffuseTexture, vec2(i-1,j+1)).r) {
				minR = texture2D(diffuseTexture, vec2(i-1,j+1)).r;
			}
			if (texture2D(diffuseTexture, vec2(i,j+1)).r) {
				minR = texture2D(diffuseTexture, vec2(i,j+1)).r;
			}
			if (texture2D(diffuseTexture, vec2(i+1,j+1)).r) {
				minR = texture2D(diffuseTexture, vec2(i+1,j+1)).r;
			}
	*/
	
	gl_FragColor.rgb = erode;
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