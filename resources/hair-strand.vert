#version 410

layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec3 vertex_normal;
layout (location = 2) in vec2 vertex_texcoord;

out vec2 texCoord;

void main(void) {
	texCoord = vertex_texcoord;
	gl_Position = vec4(vertex_position, 1.0);
}
