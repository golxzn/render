#version 330 core
out vec4 FragColor;

in vec3 UV;

uniform samplerCube u_texture;

void main() {
	// FragColor = texture(u_texture, vec3(-UV.x, -UV.y, UV.z));
	FragColor = texture(u_texture, UV);
}
