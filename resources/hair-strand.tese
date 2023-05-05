#version 410

layout(isolines) in;

uniform mat4 projection_mat, view_mat, model_mat;

in vec2 vertexTexCoord[];

out vec2 textCoord;

float rand(vec2 co){
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

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

	float offset = rand(vec2(gl_TessCoord.y, 1.0));
	float d = 1.2;
	cubicBezierInterpolation = vec3(cubicBezierInterpolation.x + offset * d, cubicBezierInterpolation.y + offset * d, cubicBezierInterpolation.z + offset * d);
	gl_Position = projection_mat * view_mat * model_mat * vec4(cubicBezierInterpolation.xyz, 1.0);
	textCoord = vertexTexCoord[gl_PrimitiveID];
}