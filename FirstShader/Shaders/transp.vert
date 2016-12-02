varying vec3 normal, position;
varying vec3 vertex_color;

void main()
{
  gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * gl_Vertex;
 
  normal = normalize(gl_NormalMatrix * gl_Normal);
 
  position = ftransform();

  vertex_color = gl_Normal.zxz;
}