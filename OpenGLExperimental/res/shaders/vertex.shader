#version 460

layout (location = 0) in vec4 vPosition;
layout (location = 1) in vec4 vColor;

uniform mat4 matrix;

out vec4 vertexColor;

void main(){
	gl_Position = matrix * vPosition;
	vertexColor = vColor;
}
