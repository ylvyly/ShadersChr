varying vec3 position; 

void main()
{
	vec2 col = (noise (position.xy) * 0.5) + 0.5; //noise values are produced between -1 and 1 => map them to [0,1]
	float val = (col.x + col.y) / 2.0; //convert values to 'fake' greyscale
	
	gl_FragColor = gl_Color * vec4(val,val,val,1);
	gl_FragColor.a = 1.0; //not transparent
}