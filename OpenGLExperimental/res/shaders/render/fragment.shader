#version 460

struct Light {
	vec3 position;
	vec3 color;
};
uniform Light light;

struct Material {
	vec3 Ka;
	vec3 Kd;
	vec3 Ks;
	float Ns;
	sampler2D texture1;
};
uniform Material material;

in vec3 vertexPosition;
in vec2 vertexTextureCoordinate;
in vec3 vertexNormal;

out vec4 fColorOut;


uniform vec3 cameraPos;

uniform int typeDraw;
uniform vec3 color;

void main(){
	if (typeDraw == 1) {
		//ambient
		vec3 ambientFinal = material.Ka * light.color;

		//diffuse
		vec3 posToLightDirVec = normalize(light.position - vertexPosition);
		vec3 diffuseColor = light.color;
		float diffuse = clamp(dot(posToLightDirVec, vertexNormal), 0, 1);
		vec3 diffuseFinal = material.Kd * diffuseColor * diffuse;

		//specular
		vec3 lightToPosDirVec = normalize(vertexPosition - light.position);
		vec3 reflectDirVec = normalize(reflect(lightToPosDirVec, normalize(vertexNormal)));
		vec3 posToViewDirVec = normalize(cameraPos - vertexPosition);
		float specularConstant = pow(max(dot(posToViewDirVec, reflectDirVec), 0), material.Ns);
		vec3 specularFinal = material.Ks * light.color * specularConstant;

		//fColorOut = texture(texture0, vertexTextureCoordinate);
		
		fColorOut = texture(material.texture1, vertexTextureCoordinate) * (vec4(ambientFinal, 1.0f) + vec4(diffuseFinal, 1.0f) + vec4(specularFinal, 1.0f));
	}
	else if (typeDraw == 2) {
		vec4 textureColor = texture(material.texture1, vertexTextureCoordinate);
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
