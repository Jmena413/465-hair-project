#version 330

// You must specify what type of primitive is passed in. In this case triangles
layout(lines) in;

// ... and what type of primitive you are outputing and how many vertices. The geometry shader is run once
// for each primitive so we will output 4 line strips
//  -- one to draw the triangle outline with 4 vertices.
//  -- one for each hexagon centered on the triangle vertices (7 vertices each)
// ... so the total number of vertices output will be 25
layout(triangle_strip, max_vertices = 6) out;

uniform mat4 projection_mat, view_mat, model_mat;
uniform vec3 eye_world;

// Example of input from the vertex shader. Note that since the geom shader runs per primative, texCoord is an array with texCoords for each vertex of the line.
in vec2 texCoord[];

// Example of an output variable to the fragment shader
// Each time a vertex is emitted the current value of this var is sent to the fragment shader.
out vec2 gTexCoord;

void main() {
    
    for(int i=0; i < 2; i++){
        // Just like in the vertex shader, geom shaders need to set the position of the vertex in screen space. We do this by multiplying the matrices.
        // The gl_in array holds the gl_positions that were set in the vertex shader for the vertices in this triangle.
        gl_Position = projection_mat * view_mat * model_mat * gl_in[i].gl_Position;

        // Update output variable to the frag shader
        gTexCoord = texCoord[i];
        
        // Tell the gpu that we want to send this vertex along the pipeline
        EmitVertex();
    }
    vec3 tangent = gl_in[0].gl_Position.xyz - gl_in[1].gl_Position.xyz;
    vec3 eyeVec = normalize(eye_world - gl_in[0].gl_Position.xyz);
    vec3 line = normalize(cross(tangent, eyeVec));
    
    gl_Position.xyz = gl_in[1].gl_Position.xyz + line;
    gTexCoord = texCoord[0];
    gl_Position = projection_mat * view_mat * model_mat * gl_Position;
    EmitVertex();

    gl_Position = projection_mat * view_mat * model_mat * gl_in[0].gl_Position;
    gTexCoord = texCoord[0];
    EmitVertex();

    gl_Position.xyz = gl_in[0].gl_Position.xyz + line;
    gTexCoord = texCoord[0];
    gl_Position = projection_mat * view_mat * model_mat * gl_Position;
    EmitVertex();

    gl_Position.xyz = gl_in[0].gl_Position.xyz + line + tangent;
    gTexCoord = texCoord[0];
    gl_Position = projection_mat * view_mat * model_mat * gl_Position;
    EmitVertex();
    EndPrimitive();
}