#version 330 core
out vec4 FragColor;

in vec3 UV;

uniform samplerCube skybox;

void main() {
	FragColor = texture(skybox, vec3(-UV.x, -UV.y, UV.z));
}
