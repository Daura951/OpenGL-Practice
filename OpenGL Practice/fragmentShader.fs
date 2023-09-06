#version 330 core

struct Material{

    sampler2D diffuse; //Diffuse Map
    sampler2D specular; //Specular map
    float shininess; //Shininess, power in phong equation

};

struct DirectionalLight {
    vec3 direction;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight {
    vec3 position;
    vec3 direction;

    float cutoff;
    float outerCutoff;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

#define NR_POINT_LIGHTS 4

out vec4 fragColor;

in vec2 texCoords;
in vec3 normal;
in vec3 fragmentPosition;


uniform vec3 cameraPos;
uniform DirectionalLight dirLight;
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform SpotLight spotLight;
uniform Material material;

vec3 CalculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir);
vec3 CalculatePointLight(PointLight pointLight, vec3 normal, vec3 fragPos, vec3 viewDirection);
vec3 CalculateSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDirection);


void main()
{

  vec3 result = vec3(0.0);

 vec3 viewDir = normalize(cameraPos - fragmentPosition);

 vec3 norm = normalize(normal);

  result+= CalculateDirectionalLight(dirLight, norm, viewDir);

    for(int i = 0; i < NR_POINT_LIGHTS; i++)
    {
        result+=CalculatePointLight(pointLights[i], norm, fragmentPosition, viewDir);
    }

   result+=CalculateSpotLight(spotLight, norm, fragmentPosition, viewDir);

  fragColor = vec4(result, 1.0);
} 


vec3 CalculateDirectionalLight(DirectionalLight dirLight, vec3 normal, vec3 viewDir)
{
    vec3 lightDirection = normalize(-dirLight.direction);

    float diff = max(dot(normal, lightDirection), 0.0);

    vec3 reflectDir = reflect(-lightDirection, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    vec3 ambient = dirLight.ambient * vec3(texture(material.diffuse, texCoords)); //ambient strength vector * texture color
    vec3 diffuse = dirLight.diffuse * diff * vec3(texture(material.diffuse, texCoords));
    vec3 specular = dirLight.specular * spec * vec3(texture(material.specular, texCoords));

    return (ambient + diffuse + specular);
}

vec3 CalculatePointLight(PointLight pointLight, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDirection = normalize(pointLight.position - fragPos);

    float diff = max(dot(normal, lightDirection), 0.0);

    vec3 reflectDir = reflect(-lightDirection, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    float distance = length(pointLight.position - fragPos);
    float attenuation = 1.0 / ((pointLight.constant + pointLight.linear * distance) + (pointLight.quadratic * (distance * distance)));    

    vec3 ambient = pointLight.ambient * vec3(texture(material.diffuse, texCoords));
    vec3 diffuse = pointLight.diffuse * diff * vec3(texture(material.diffuse, texCoords));
    vec3 specular = pointLight.specular * spec * vec3(texture(material.specular, texCoords));

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return (ambient + diffuse + specular);
}

vec3 CalculateSpotLight(SpotLight spotLight, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDirection = normalize(spotLight.position - fragPos);

    float diff = max(dot(normal, lightDirection), 0.0);

    vec3 reflectDir = reflect(-lightDirection, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    float distance = length(spotLight.position - fragPos);
    float attenuation = 1.0 / (spotLight.constant + spotLight.linear * distance + spotLight.quadratic * (distance * distance));    
  
    float theta = dot(lightDirection, normalize(-spotLight.direction)); 
    float epsilon = spotLight.cutoff - spotLight.outerCutoff;
    float intensity = clamp((theta - spotLight.outerCutoff) / epsilon, 0.0, 1.0);

    vec3 ambient = spotLight.ambient * vec3(texture(material.diffuse, texCoords));
    vec3 diffuse = spotLight.diffuse * diff * vec3(texture(material.diffuse, texCoords));
    vec3 specular = spotLight.specular * spec * vec3(texture(material.specular, texCoords));

    ambient *= intensity;
    diffuse *= intensity;
    specular *= intensity;

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return (ambient + diffuse + specular);
}