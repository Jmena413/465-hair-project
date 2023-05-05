#version 410

layout(vertices = 4) out;

in vec2 texCoord[];

out vec2 vertexTexCoord[];

void main() {
	gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;

	gl_TessLevelOuter[0] = 10.0;
	gl_TessLevelOuter[1] = 20.0;

	vertexTexCoord[gl_InvocationID] = texCoord[gl_InvocationID];
}

	

	

	