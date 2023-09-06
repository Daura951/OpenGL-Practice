#version 330 core

struct Material{

    sampler2D diffuse; //Diffuse Map
    sampler2D specular; //Specular map
    float shininess; //Shininess, power in phong equation

};

struct Light{
   vec3 position;

   vec3 ambient; //intensity of ambient
   vec3 diffuse; //intensity of diffuse
   vec3 specular; //intensity of specular

   float constant;
   float linear;
   float quadratic;
};



out vec4 fragColor;

in vec2 texCoords;
in vec3 normal;
in vec3 fragmentPosition;


uniform vec3 lightPos;
uniform vec3 cameraPos;
uniform Material material;
uniform Light light;

void main()
{
    
    //ATTENUATION
    float distance = length(light.position - fragmentPosition);
    float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance*distance));
    
    //AMBIENT
    float ambientStrength = 0.1f;
    vec3 ambientLight = light.ambient * vec3(texture(material.diffuse, texCoords)); //This gives a little light that can shine off the object

    //DIFFUSE
    vec3 norm = normalize(normal); //normalize normal
    vec3 lightDirection = normalize(lightPos - fragmentPosition); //subtract where our ray starts to where it ends (surface of object) to get the drection
    float diffuseAmt = max(dot(norm, lightDirection), 0.0f); //dot the normal and light direction to determine how intense the light is. 1 if it's on angle to 0 if perpendicular.
    vec3 diffuseLight = light.diffuse * diffuseAmt * vec3(texture(material.diffuse, texCoords)); //Apply to our basic light also get the color of texture at coordinate to determine diffuse color

    //SPECULAR
    vec3 viewerDirection = normalize(cameraPos - fragmentPosition); //view direction similar to lgiht direction
    vec3 reflectVec = reflect(-lightDirection, norm); //reflect -lightDir to mimic light reflection
    float specularAmt = pow(max(dot(viewerDirection, reflectVec), 0.0f), material.shininess); //specular power
    vec3 specularLight = light.specular * specularAmt * vec3(texture(material.specular, texCoords));

    ambientLight*=attenuation;
    diffuseLight*=attenuation;
    specularLight*=attenuation;

    fragColor = vec4((ambientLight + diffuseLight + specularLight) , 1.0f); //phong equation * objectColor
}