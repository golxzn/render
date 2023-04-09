#version 330 core

in vec2 UV;

uniform sampler2D diffuse0;

out vec4 FragColor;

void main() {
	FragColor = texture(diffuse0, vec2(-UV.x, -UV.y));
}
