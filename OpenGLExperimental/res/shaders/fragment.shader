#version 460

in vec4 vertexNormal;
in vec2 vertexTextureCoordinate;

out vec4 fColorOut;

uniform sampler2D texture0;
uniform int typeDraw;
uniform vec3 color;

void main(){
	if (typeDraw == 1) {
		fColorOut = texture(texture0, vertexTextureCoordinate);
	}
	else if (typeDraw == 2) {
		vec4 textureColor = texture(texture0, vertexTextureCoordinate);
		vec4 vecColor = vec4(color, 0.5);
		fColorOut = textureColor * vecColor;
	}
	else if (typeDraw == 3) {
		fColorOut = vec4(0.33, 0.33, 0.33, 1);
	}
	else if (typeDraw == 4) {
		fColorOut = vec4(color, 0.5);
	}
}
