#version 410

layout(isolines) in;

float lagrangeInterpolate(float x, vec2 v1, vec2 v2, vec2 v3, vec2 v4) {
	float term1 = v1.y * (x - v2.x) * (x - v3.x) * (x - v4.x) / (v1.x - v2.x) * (v1.x - v3.x)* (v1.x - v4.x);
	float term2 = v2.y * (x - v1.x) * (x - v3.x) * (x - v4.x) / (v2.x - v1.x) * (v2.x - v3.x)* (v2.x - v4.x);
	float term3 = v3.y * (x - v1.x) * (x - v2.x) * (x - v4.x) / (v3.x - v1.x) * (v3.x - v2.x)* (v3.x - v4.x);
	float term4 = v4.y * (x - v1.x) * (x - v2.x) * (x - v3.x) / (v4.x - v1.x) * (v4.x - v2.x)* (v4.x - v3.x);
	float interpolatedY = term1 + term2 + term3 + term4;
	return interpolatedY;
}

void main() {
	
}