#version 410

layout(isolines) in;

in vec2 vertexTexCoord[];

out vec2 texCoord[];

void main() {
	//cubic Bezier interpolation
	vec4 controlPoint1 = gl_in[0].gl_Position;
	vec4 controlPoint2 = gl_in[1].gl_Position;
	vec4 controlPoint3 = gl_in[2].gl_Position;
	vec4 controlPoint4 = gl_in[3].gl_Position;

	mat4 splineMatrix;
	splineMatrix[0] = vec4(1, -3, 3, 1);
	splineMatrix[1] = vec4(0, 3, -6, 3);
	splineMatrix[2] = vec4(0, 0, 3, -3);
	splineMatrix[3] = vec4(0, 0, 0, 1);

	vec4 monomialBasis = vec4(1, gl_Position.x, pow(gl_Position.x, 2), pow(gl_Position.x, 3));

	vec4 sMxmB = splineMatrix * monomialBasis;

	gl_Position = controlPoint1 * sMxmB[0] + controlPoint2 * sMxmB[1] + controlPoint3 * sMxmB[2] + controlPoint4 * sMxmB[3];
	texCoord[gl_PrimitiveID] = texCoord[gl_PrimitiveID];
}