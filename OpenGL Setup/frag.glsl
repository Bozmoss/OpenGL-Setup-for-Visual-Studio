#version 330 core

out vec4 FragColor;

in vec3 FragPos, Normal;

uniform vec3 lightPos, lightColor, objectColor, viewPos;
uniform int isLight;

void main() {
	vec3 result = objectColor;
	if (isLight == 0) {
		vec3 norm = normalize(Normal);

		vec3 lightDir = normalize(lightPos - FragPos);

		float ambientStrength = 0.2;
		vec3 ambient = ambientStrength * lightColor;

		float diff = max(dot(norm, lightDir), 0.0);
		vec3 diffuse = diff * lightColor;

		float specularStrength = 0.5;
		vec3 viewDir = normalize(viewPos - FragPos);
		vec3 reflectDir = reflect(-lightDir, norm);
		float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
		vec3 specular = specularStrength * spec * lightColor;

		result = (ambient + diffuse + specular) * objectColor;
	}

	FragColor = vec4(result, 1.0);
}