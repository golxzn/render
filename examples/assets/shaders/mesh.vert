#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aUV;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out vec2 UV;
out vec3 v_normal;
out vec3 v_position;
out vec3 v_view_position;

void main() {
	UV = aUV;
	vec4 world_position = model * vec4(aPos, 1.0);
	v_normal = normalize((model * vec4(aNormal, 0.0)).xyz);
	v_position = world_position.xyz;
	v_view_position = (view * world_position).xyz;
	gl_Position = projection * view * world_position;
}
