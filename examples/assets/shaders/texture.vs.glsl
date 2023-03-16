#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aUV;

out vec4 pFragColor;
out vec2 pUV;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main() {
	pFragColor = vec4(aColor, 1.0);
	pUV = aUV;
	gl_Position = projection * view * model * vec4(aPos, 1.0);
}
