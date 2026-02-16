#version 400 core

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexColor;

out vec3 Color;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

void main()
{
    Color = VertexColor;
    
    // Standard MVP transformation
    // Note: Matrix multiplication is Right-to-Left
    mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
    
    gl_Position = MVP * vec4(VertexPosition, 1.0);
}
