#version 460

in vec3 vertexColor;

out vec4 fColorOut;

void main(){
	fColorOut = vec4(vertexColor, 1.0f);
}
