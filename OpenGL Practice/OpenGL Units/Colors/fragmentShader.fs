#version 330 core

out vec4 fragColor;


in vec2 texCoord;


uniform vec3 objectColor;
uniform vec3 lightColor;

void main()
{
    fragColor = vec4(objectColor * lightColor, 1.0f);
}