#version 330 core

uniform mat4 u_view;
uniform mat4 u_projection;

out float f_near;
out float f_far;
out vec3 f_near_point;
out vec3 f_far_point;
out mat4 f_view;
out mat4 f_projection;

vec3 grid_plane[6] = vec3[](
	vec3(1, 1, 0), vec3(-1, -1, 0), vec3(-1, 1, 0),
	vec3(-1, -1, 0), vec3(1, 1, 0), vec3(1, -1, 0)
);

vec3 unproject_point(float x, float y, float z, mat4 inv_view, mat4 inv_proj) {
	vec4 unproj_point = inv_view * inv_proj * vec4(x, y, z, 1.0);
	return unproj_point.xyz / unproj_point.w;
}

void main() {
	mat4 inv_view = inverse(u_view);
	mat4 inv_proj = inverse(u_projection);
	vec3 point = grid_plane[gl_VertexID];

	f_near = 0.01;
	f_far = 100.0;
	f_near_point = unproject_point(point.x, point.y, 0.0, inv_view, inv_proj);
	f_far_point = unproject_point(point.x, point.y, 1.0, inv_view, inv_proj);
	f_view = u_view;
	f_projection = u_projection;

	gl_Position = vec4(point, 1.0);
}
