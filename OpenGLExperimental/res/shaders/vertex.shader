#version 460

layout (location = 0) in vec4 vPosition;
layout (location = 1) in vec2 vTextureCoordinate;
layout (location = 2) in vec4 vNormal;

out vec2 vertexTextureCoordinate;
out vec4 vertexNormal;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main(){
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vPosition;
	
	//passando para o fragment_shader
	vertexTextureCoordinate = vec2(vTextureCoordinate.x, vTextureCoordinate.y * -1.f);
	vertexNormal = vNormal;
}
