#version 460

layout (location = 0) in vec4 vPosition;
layout (location = 1) in vec4 vNormal;
layout (location = 2) in vec2 vTextureCoordinate;
layout (location = 3) in vec4 vColor;

out vec4 vertexNormal;
out vec2 vertexTextureCoordinate;
out vec4 vertexColor;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main(){
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vPosition;
	
	//passando para o fragment_shader
	vertexNormal = vNormal;
	vertexTextureCoordinate = vTextureCoordinate;
	vertexColor = vColor;
}
