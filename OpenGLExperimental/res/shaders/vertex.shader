#version 460

layout (location = 0) in vec4 vPosition;
layout (location = 1) in vec4 vColor;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

out vec4 vertexColor;

void main(){
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vPosition;
	vertexColor = vColor;
}
