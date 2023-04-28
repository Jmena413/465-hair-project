#version 410

uniform sampler2D textureSampler;

in vec2 gTexCoord;

out vec4 fragColor;

void main() {
	fragColor = texture(textureSampler, gTexCoord);
	fragColor = vec4(0.0, 0.0, 0.0, 1.0);
}