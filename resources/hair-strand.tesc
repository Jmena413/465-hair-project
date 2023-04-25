#version 410

layout(vertices = 4) out;

void main() {
	gl_TessLevelOuter[0] = 10;
	gl_TessLevelOuter[1] = 12;

	gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
}