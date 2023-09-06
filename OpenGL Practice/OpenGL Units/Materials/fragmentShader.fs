#version 330 core

struct Material{

    vec3 ambient; //ambient vector (ambient color)
    vec3 diffuse; //diffuse vector (object color)
    vec3 specular; //Specular vector
    float shininess; //Shininess, power in phong equation

};

struct Light{
   vec3 ambient; //intensity of ambient
   vec3 diffuse; //intensity of diffuse
   vec3 specular; //intensity of specular
   vec3 position;
};



out vec4 fragColor;


in vec3 normal;
in vec3 fragmentPosition;


uniform vec3 lightPos;
uniform vec3 cameraPos;
uniform Material material;
uniform Light light;

void main()
{
    //AMBIENT
    float ambientStrength = 0.1f;
    vec3 ambientLight = light.ambient * material.ambient; //This gives a little light that can shine of the object

    //DIFFUSE
    vec3 norm = normalize(normal); //normalize normal
    vec3 lightDirection = normalize(lightPos - fragmentPosition); //subtract where our ray starts to where it ends (surface of object) to get the drection
    float diffuseAmt = max(dot(norm, lightDirection), 0.0f); //dot the normal and light direction to determine how intense the light is. 1 if it's on angle to 0 if perpendicular.
    vec3 diffuseLight = light.diffuse* (diffuseAmt * material.diffuse); //Apply to our basic light

    //SPECULAR
    vec3 viewerDirection = normalize(cameraPos - fragmentPosition); //view direction similar to lgiht direction
    vec3 reflectVec = reflect(-lightDirection, norm); //reflect -lightDir to mimic light reflection
    float specularAmt = pow(max(dot(viewerDirection, reflectVec), 0.0f), material.shininess); //specular power
    vec3 specularLight = light.specular*(material.specular * specularAmt);


    fragColor = vec4((ambientLight + diffuseLight + specularLight) , 1.0f); //phong equation * objectColor
}