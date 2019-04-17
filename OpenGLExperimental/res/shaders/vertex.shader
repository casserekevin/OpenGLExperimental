#version 460

layout (location = 0) in vec4 vPosition;
layout (location = 1) in vec4 vColor;
layout (location = 2) in vec2 vTextureCoordinate;

out vec4 vertexColor;
out vec2 vertexTextureCoordinate;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main(){
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vPosition;
	
	//passando para o fragment_shader
	vertexColor = vColor;
	vertexTextureCoordinate = vTextureCoordinate;
}
