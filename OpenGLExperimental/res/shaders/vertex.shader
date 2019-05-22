#version 460

layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec2 vTextureCoordinate;
layout (location = 2) in vec3 vNormal;

out vec3 vertexPosition;
out vec2 vertexTextureCoordinate;
out vec3 vertexNormal;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main(){
	//passando para o fragment_shader
	vertexPosition = (modelMatrix * vec4(vPosition, 1.0f)).xyz;
	vertexTextureCoordinate = vec2(vTextureCoordinate.x, vTextureCoordinate.y * -1.f);
	vertexNormal = mat3(modelMatrix) * vNormal;
	
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(vPosition, 1.0f);
}
