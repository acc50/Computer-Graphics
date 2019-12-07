#version 330 core
in vec3 FragPos;
in vec3 Normal;
out vec4 Fragcolor;

uniform vec3 viewPos;
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 objectColor;

void main()
{
	vec3 ambientLight = vec3(0.2);
	vec3 ambient = ambientLight * lightColor;

	vec3 normalVector = Normal;
	vec3 lightDir = normalize(lightPos - FragPos);
	float diffuseLight = max(dot(normalVector, lightDir), 0.0);
	vec3 diffuse = diffuseLight * lightColor;

	int shininess = 32;
	vec3 viewDir = normalize(FragPos - viewPos);
	vec3 reflectDir =  reflect(lightDir, normalVector);
	float specularColor = max(dot(viewDir, reflectDir), 0.0);
	specularColor = pow(specularColor, shininess);
	vec3 specular = specularColor * lightColor;

	vec3 result = (ambient + diffuse + specular) * objectColor;

	Fragcolor = vec4(result, 1.0);
}