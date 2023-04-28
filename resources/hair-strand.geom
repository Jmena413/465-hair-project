#version 410

layout(lines) in;

layout(triangle_strip, max_vertices = 4) out;

uniform mat4 projection_mat, view_mat, model_mat;
uniform vec3 eye_world;

in vec2 textCoord[];

out vec2 gTexCoord;

void main() {
    vec3 tangent = gl_in[0].gl_Position.xyz - gl_in[1].gl_Position.xyz;
    vec3 topEyeVec = normalize(eye_world - gl_in[0].gl_Position.xyz);
    vec3 botEyeVec = normalize(eye_world - gl_in[1].gl_Position.xyz);
    vec3 topLine = normalize(cross(tangent, topEyeVec));
    vec3 botLine = normalize(cross(tangent, botEyeVec));
    float hairWidth = 0.025;
    
    gl_Position.xyz = gl_in[1].gl_Position.xyz - hairWidth * botLine;
    gl_Position.w = 1.0;
    gTexCoord = textCoord[1];
    gl_Position = projection_mat * view_mat * model_mat * gl_Position;
    EmitVertex();

    gl_Position.xyz = gl_in[1].gl_Position.xyz + hairWidth * botLine;
    gl_Position.w = 1.0;
    gTexCoord = textCoord[1];
    gl_Position = projection_mat * view_mat * model_mat * gl_Position;
    EmitVertex();

    gl_Position.xyz = gl_in[0].gl_Position.xyz - hairWidth * topLine;
    gl_Position.w = 1.0;
    gTexCoord = textCoord[0];
    gl_Position = projection_mat * view_mat * model_mat * gl_Position;
    EmitVertex();

    gl_Position.xyz = gl_in[0].gl_Position.xyz + hairWidth * topLine;
    gl_Position.w = 1.0;
    gTexCoord = textCoord[0];
    gl_Position = projection_mat * view_mat * model_mat * gl_Position;
    EmitVertex();
    EndPrimitive();
}