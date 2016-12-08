varying vec2 vTexCoord;

in vec3 Points;
in vec2 Texcoords;
in vec3 Tangent;
in vec3 Normal;

varying vec2 vN;

varying vec3 lightvec, n, lightDir, e, pos;

void main()
{	
	vTexCoord = Texcoords;
	
	mat3 NormalMatrix = inverse(transpose(gl_ModelViewMatrix)); //gl_NormalMatrix;
	n = normalize( NormalMatrix * Normal ); //surface normal
	pos = vec3 (gl_ModelViewMatrix * gl_Vertex); //view plane vector
	
	//vec4 eyeNorm = normalize(gl_ModelViewMatrix * vec4(Normal, 0.0));
	e = normalize( vec3(gl_ModelViewMatrix * gl_Vertex)); //view plane vector
	
	//for extension 
	lightDir = normalize(gl_LightSource[0].position.xyz - pos); //e instead of pos?
	//vec3 angle = acos( dot(e,lightDir) / (length(e) * length(lightDir)));
	//float f = sqrt(e.x*e.x + e.z*e.z);
	//float theta = atan(e.z/e.x);
	//vec3 newE = (f * cos(theta + angle), f * sin(theta + angle));
	
	//light space normals
	vec3 nlx = n * lightDir.x;
	vec3 nly = n * lightDir.y;
	vec3 nlz = n * lightDir;
	
	// r (brightness) and theta (light view angle) from extension paper
	float rad = acos (nlz) / 3.1415926535897932384626433832795 ;
	float tan = atan (nly / nlx);
	
	//calculate cartesian coordinates
	float u = rad * cos (tan);
	float v = rad * sin (tan);
	
	vec2 rxy = vec2(rad,tan);
	vec2 uv = vec2(u, v);
	
	float m2 = 2.0 * sqrt( pow(rad,2) + pow(tan,2));
	float m3 = 2.0 * sqrt( pow(u,2) + pow(v,2));

	
	
	vec3 r = reflect(lightDir,n);
	float m = 2.0 * sqrt( pow(r.x,2) + pow(r.y,2) + pow(r.z,2)); //double length of r
	vN = (r.xy / m) + 0.5;
	
	//for phong highlight
	vec3 vDash = reflect (e,n);
	vec3 vlx = n * vDash.x;
	vec3 vly = n * vDash.y;
	vec3 vlz = n * vDash;
	lightvec = normalize (e + lightDir);
	

	//gl_Position = gl_ModelViewProjectionMatrix * p;
	gl_Position = ftransform(); //gl_ModelViewProjectionMatrix * gl_Vertex;

}
/*
//rotation matrix
	//calculate rotation matrix
	vec3 u = normalize (cross(e,lightDir)); // u is the rotation axis
	float r1 = cos(angle) + (u.x * u.x) * (1-cos(angle)); 
	float r2 = u.x * u.y * (1-cos(angle)) - u.z * sin(angle);
	float r3 = u.x * u.z * (1-cos(angle)) + u.y * sin(angle);
	float r4 = u.y * u.x * (1-cos(angle)) + u.z * sin(angle);
	float r5 = cos(angle) + (u.y * u.y) * (1-cos(angle));
	float r6 = u.y * u.z * (1-cos(angle)) - u.x * sin(angle);
	float r7 = u.z * u.x * (1-cos(angle)) - u.y * sin(angle);
	float r8 = u.z * u.y * (1-cos(angle)) + u.x * sin(angle);
	float r9 = cos(angle) + (u.z * u.z) * (1-cos(angle));
	
	mat3 rotMatrix;
	rotMatrix[0] = vec3(r1,r4,r7);
	rotMatrix[1] = vec3(r2,r5,r8);
	rotMatrix[2] = vec3(r3,r6,r9);
	//now rotMatrix * e = lightDir
	//vec3 newE = normalize( vec3(rotMatrix * e));
*/

/*
//for rim lighting
	float a = acos(n*e);
	float hr = 0.7 * smoothstep(abs(a), 0.5, 0);
	float rx = smoothstep (hr, -1,0) * (r-1)+1 ;
	float ry = (1 - smoothstep (hr, 0,1)) * r ;
	vec2 rxy2 = vec2 (rx, ry);
	float m3 = 2.0 * sqrt( pow(rx,2) + pow(ry,2));
	vN = (rxy2 / m3) + 0.5;
*/




