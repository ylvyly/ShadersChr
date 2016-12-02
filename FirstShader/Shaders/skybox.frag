



in vec3 texcoords;
uniform samplerCube cube_texture;
out vec4 frag_colour;

void main() {
  gl_FragColor = texture(cube_texture, texcoords);
}