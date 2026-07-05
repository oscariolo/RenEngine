#version 330 core
#define MAX_LIGHTS 10

out vec4 FragColor;

in vec3 Normal;  
in vec3 FragPos;
in vec3 ourColor;  
  
uniform vec3 viewPos; 
uniform vec3 lightColor = vec3(1.0, 1.0, 1.0);

uniform float ambientStrength = 0.6;
uniform float specularStrength = 0.5;
uniform float shininess = 32.0;
uniform int numLights = 0;

struct PointLight{
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform PointLight pointLights[MAX_LIGHTS];

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
    
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    if(numLights == 0)
    {
        FragColor = vec4(ambientStrength * lightColor, 1.0);
    }
    else
    {
        for(int i = 0; i < numLights; i++){
            FragColor += vec4(CalcPointLight(pointLights[i], norm, FragPos, viewDir), 1.0);
        }
    }


}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)

{
        //ambient
    vec3 ambient = ambientStrength * light.ambient;
  	
    // diffuse 
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * light.diffuse;
    
    // specular
    vec3 reflectDir = reflect(-lightDir, normal);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = specularStrength * spec * light.specular;  

    return (ambient + diffuse + specular);
}
