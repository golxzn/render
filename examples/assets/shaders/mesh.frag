#version 330 core

in vec2 UV;

uniform sampler2D diffuse0;
uniform sampler2D diffuse1;

out vec4 FragColor;

void main() {
	vec4 result_color;
	if (UV.x <= 0.0 || UV.x >= 1.0) {
		result_color = texture(diffuse0, UV);
	} else {
		result_color = texture(diffuse1, UV);
	}
	FragColor = result_color;

}
