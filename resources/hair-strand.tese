#version 410

layout(isolines) in;

uniform mat4 projection_mat, view_mat, model_mat;

in vec2 vertexTexCoord[];

out vec2 textCoord;

void main() {
	//cubic Bezier interpolation
	vec3 controlPoint1 = gl_in[0].gl_Position.xyz;
	vec3 controlPoint2 = gl_in[1].gl_Position.xyz;
	vec3 controlPoint3 = gl_in[2].gl_Position.xyz;
	vec3 controlPoint4 = gl_in[3].gl_Position.xyz;

	float u = gl_TessCoord.x;

	float mB1 = (1.0 - u) * (1.0 - u) * (1.0 - u);
	float mB2 = 3.0 * u * (1.0 - u) * (1.0 - u);
	float mB3 = 3.0 * u * u * (1.0 - u);
	float mB4 = u * u * u;

	vec3 cubicBezierInterpolation = controlPoint1 * mB1 + controlPoint2 * mB2 + controlPoint3 * mB3 + controlPoint4 * mB4;

	gl_Position = projection_mat * view_mat * model_mat * vec4(cubicBezierInterpolation.xyz, 1.0);

	//offset calculation
	/*
	vec4 v1 = normalize(gl_in[0].gl_Position - gl_in[1].gl_Position);
	vec4 v2 = normalize(vec4(1.0, 1.0, (-v1.x - v1.y)/v1.z , 1.0));
	vec4 v3 = normalize(vec4(cross(v1.xyz, v2.xyz), 1.0));

	float r = max(0.5, noise1(gl_Position.x));
	float theta = noise1(gl_Position.x) * 2 * 3.1415927;

	gl_Position.x += r * cos(theta) * v2.x + r * sin(theta) * v3.x;
	gl_Position.y += r * cos(theta) * v2.y + r * sin(theta) * v3.y;
	gl_Position.z += r * cos(theta) * v2.z + r * sin(theta) * v3.z;
	gl_Position.w = 1.0;
	*/

	float offset = gl_TessCoord.y;
	gl_Position.x += offset * 0.8;
	gl_Position.y += offset * 0.8;

	textCoord = vertexTexCoord[gl_PrimitiveID];
}