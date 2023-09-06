#version 330 core

struct Material{

    vec3 ambient; //ambient strength (object color)
    vec3 diffuse; //diffuse strength (object color)
    vec3 specular; //specular strength
    float shininess;
};



out vec4 fragColor;


in vec3 normal;
in vec3 fragmentPosition;


uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 cameraPos;
uniform Material material;

void main()
{
    //AMBIENT
    vec3 ambientLight = vec3(0.1f) * material.ambient; //This gives a little white that can shine of the object

    //DIFFUSE
    vec3 norm = normalize(normal); //normalize normal
    vec3 lightDirection = normalize(lightPos - fragmentPosition); //subtract where our ray starts to where it ends (surface of object) to get the drection
    float diffuseAmt = max(dot(norm, lightDirection), 0.0f); //dot the normal and light direction to determine how intense the light is. 1 if it's on angle to 0 if perpendicular.
    vec3 diffuseLight = vec3(0.5f) * (material.diffuse * diffuseAmt) ; //Apply to our basic light

    //SPECULAR
    vec3 viewerDirection = normalize(cameraPos - fragmentPosition); //view direction similar to lgiht direction
    vec3 reflectVec = reflect(-lightDirection, norm); //reflect -lightDir to mimic light reflection
    float specularAmt = pow(max(dot(viewerDirection, reflectVec), 0.0f), material.shininess); //specular power
    vec3 specularLight = vec3(1.0f) * (specularAmt * material.specular);


    fragColor = vec4((ambientLight + diffuseLight + specularLight), 1.0f); //phong equation
 }