#version 330

/** Diffuse/ambient surface color */
uniform sampler2D textureSampler;

/** Texture coordinate */
in vec2 gTexCoord;

out vec4 fragColor;

void main() {
	fragColor = texture(textureSampler, gTexCoord);
}