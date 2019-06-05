#version 460

layout (location = 0) in vec3 vPosition;

uniform mat4 projectionMatrix;

void main(){
	//passando para o fragment_shader
	
	gl_Position = projectionMatrix * vec4(vPosition, 1.0f);
}
