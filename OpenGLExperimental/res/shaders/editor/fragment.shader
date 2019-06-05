#version 460

out vec4 fColorOut;

uniform vec3 color;

void main(){
	fColorOut = vec4(color, 1.0f);
}
