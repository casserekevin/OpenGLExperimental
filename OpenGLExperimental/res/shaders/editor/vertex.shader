#version 460

layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vColor;

out vec3 vertexColor;

uniform mat4 projectionMatrix;

void main(){
	//passando para o fragment_shader
	
	vertexColor = vColor;
	gl_Position = projectionMatrix * vec4(vPosition, 1.0f);
}
