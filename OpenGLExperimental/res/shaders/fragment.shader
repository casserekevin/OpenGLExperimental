#version 460

in vec4 vertexNormal;
in vec2 vertexTextureCoordinate;
in vec4 vertexColor;

out vec4 fColorOut;

uniform sampler2D texture0;

uniform int typeDraw;

//vec4 colorCalculation() {
//	if (all(equal(vertexTextureCoordinate, vec2(-1, -1))) && all(equal(vertexColor, vec4(-2, -2, -2, 1)))) {
//		return vec4(0.33, 0.33, 0.33, 1);
//		/*return texture(texture0, vertexTextureCoordinate) * vec4(vertexColor, 1);*/
//	}
//	else if (all(equal(vertexTextureCoordinate, vec2(-1, -1))) && any(notEqual(vertexColor, vec4(-2, -2, -2, 1)))) {
//		return vertexColor;
//	}
//	/*
//	else if (all(notEqual(vertexTextureCoordinate, vec2(-1, -1))) && all(equal(vertexColor, vec4(-1, -1, -1, -1)))) {
//		return texture(texture0, vertexTextureCoordinate);
//	}
//	else {
//		return vec4(0.33, 0.33, 0.33, 1);
//	}*/
//}

void main(){
	//fColorOut = vertexColor;
	//fColorOut = colorCalculation();
	//fColorOut = texture(texture0, vertexTextureCoordinate);
	if (typeDraw == 1) {
		fColorOut = texture(texture0, vertexTextureCoordinate);
	}
	else if (typeDraw == 2) {
		fColorOut = texture(texture0, vertexTextureCoordinate) * vertexColor;
	}
	else if (typeDraw == 3) {
		fColorOut = vec4(0.33, 0.33, 0.33, 1);
	}
	else if (typeDraw == 4) {
		fColorOut = vertexColor;
	}
}
