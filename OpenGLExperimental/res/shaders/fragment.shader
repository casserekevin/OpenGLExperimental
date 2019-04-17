#version 460

in vec4 vertexColor;
in vec2 vertexTextureCoordinate;

out vec4 fColorOut;

uniform sampler2D texture0;

void main(){
	//fColorOut = vertexColor;
	fColorOut = texture(texture0, vertexTextureCoordinate) * vertexColor;
}
