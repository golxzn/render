#version 330 core

in float f_near;
in float f_far;
in vec3 f_near_point;
in vec3 f_far_point;
in mat4 f_view;
in mat4 f_projection;

layout(location = 0) out vec4 fragment_color;

vec4 grid(vec3 fragPos3D, float scale, bool draw_axis) {
	vec2 coord = fragPos3D.xz * scale; // use the scale variable to set the distance between the lines
	vec2 derivative = fwidth(coord);
	vec2 grid = abs(fract(coord - 0.5) - 0.5) / derivative;
	float line = min(grid.x, grid.y);
	float minimumz = min(derivative.y, 1);
	float minimumx = min(derivative.x, 1);
	vec4 color = vec4(0.2, 0.2, 0.2, 1.0 - min(line, 1.0));
	if (!draw_axis)
		return color;

	// z axis
	if(fragPos3D.x > -0.1 * minimumx && fragPos3D.x < 0.1 * minimumx)
		color.z = 1.0;
	// x axis
	if(fragPos3D.z > -0.1 * minimumz && fragPos3D.z < 0.1 * minimumz)
		color.x = 1.0;
	return color;
}

float compute_depth(vec3 pos) {
	vec4 clip_space_pos = f_projection * f_view * vec4(pos, 1.0);
	return (clip_space_pos.z / clip_space_pos.w);
}

float compute_linear_depth(vec3 pos) {
	vec4 clip_space_pos = f_projection * f_view * vec4(pos, 1.0);
	float clip_space_depth = (clip_space_pos.z / clip_space_pos.w) * 2.0 - 1.0; // put back between -1 and 1
	float linear_depth = (2.0 * f_near * f_far) / (f_far + f_near - clip_space_depth * (f_far - f_near)); // get linear value between 0.01 and 100
	return linear_depth / f_far; // normalize
}

void main() {
	float transparency = -f_near_point.y / (f_far_point.y - f_near_point.y);
	vec3 frag_pos_3d = f_near_point + transparency * (f_far_point - f_near_point);

	gl_FragDepth = compute_depth(frag_pos_3d);

	float linear_depth = compute_linear_depth(frag_pos_3d);
	float fading = max(0, (0.5 - linear_depth));

	fragment_color = (grid(frag_pos_3d, 10, true) + grid(frag_pos_3d, 1, false)) * float(transparency > 0);
	fragment_color.a *= fading;
}