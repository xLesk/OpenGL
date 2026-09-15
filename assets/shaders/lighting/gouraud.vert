#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 normalModel;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

uniform float ambientStrength;
uniform float specularStrength;

out vec3 vertexColor;

void main() {
  vec3 FragPos = vec3(model * vec4(aPos, 1.0));

  vec3 norm = normalize(normalModel * aNormal);

  // ambient
  vec3 ambient = ambientStrength * lightColor;

  // diffuse
  vec3 lightDir = normalize(lightPos - FragPos);
  float diff = max(dot(norm, lightDir), 0.0);
  vec3 diffuse = diff * lightColor;

  // specular
  vec3 viewDir = normalize(viewPos - FragPos);
  vec3 reflectDir = reflect(-lightDir, norm);

  float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
  vec3 specular = specularStrength * spec * lightColor;

  vertexColor = (ambient + diffuse + specular) * objectColor;

  gl_Position = projection * view * model * vec4(aPos, 1.0);
}
