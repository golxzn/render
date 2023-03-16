#version 330 core

in vec4 pFragColor;
in vec2 pUV;

uniform sampler2D diffuse0;
uniform sampler2D diffuse1;

out vec4 FragColor;

void main() {
	vec4 result_color;
	if (pUV.x <= 0.0 || pUV.x >= 1.0) {
		result_color = texture(diffuse0, pUV);
	} else {
		result_color = texture(diffuse1, pUV);
	}
	FragColor = result_color;

}
