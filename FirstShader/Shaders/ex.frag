varying vec3 lightDir, normal;

void main()
{
	float intensity;
	vec3 n;
	vec4 color;

	n = normalize(normal);

	intensity = max(dot(lightDir,n),0.0); 
	color = vec4(0.5, 1.0, 0.5, 1) * intensity;

	gl_FragColor = color;
}

