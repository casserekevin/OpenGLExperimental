#version 460

layout (location = 0) in vec4 vPosition;
layout (location = 1) in vec4 vNormal;
layout (location = 2) in vec2 vTextureCoordinate;

out vec4 vertexNormal;
out vec2 vertexTextureCoordinate;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main(){
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vPosition;
	
	//passando para o fragment_shader
	vertexNormal = vNormal;
	vertexTextureCoordinate = vec2(vTextureCoordinate.x, vTextureCoordinate.y * -1.f);
}
