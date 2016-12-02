varying vec3 normal, position;

void main()
{
  gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * gl_Vertex;
 
  normal = normalize(gl_NormalMatrix * gl_Normal);
 
  position = ftransform();
}