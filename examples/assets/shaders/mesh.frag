#version 330 core

in vec2 UV;
in vec3 v_normal;
in vec3 v_position;
in vec3 v_view_position;

struct material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};
struct light_parameters {
	vec3 position;
	vec3 color;
	vec3 specular;
};

uniform light_parameters u_light;
uniform material u_material;
uniform sampler2D u_diffuse0;

out vec4 fragment_color;

vec3 compute_lighting(vec3 eye_direction, light_parameters light_params, vec3 diffuse) {
	vec3 light_dir = normalize(light_params.position - v_position);
	vec3 half_dir = normalize(light_dir - eye_direction);
	vec3 light = light_params.color * diffuse * max(dot(normalize(v_normal), light_dir), 0)  * u_material.diffuse;
	light += light_params.color * diffuse * pow(max(dot(normalize(v_normal), half_dir), 0), 128);

	// specular
	vec3 reflect_dir = reflect(-light_dir, normalize(v_normal));
	float spec = pow(max(dot(eye_direction, reflect_dir), 0.0), u_material.shininess);
	light += light_params.specular * (spec * u_material.specular);

	return light;
}

void main() {
	vec4 diffuse = texture(u_diffuse0, UV);
	// diffuse
	vec3 eye_dir = normalize(v_position - v_view_position).xyz;
	vec3 light = compute_lighting(eye_dir, u_light, diffuse.xyz);

	fragment_color = vec4(u_material.ambient + light, 1.0);
}
