#version 330 core
#define MAX_LIGHTS 10

out vec4 FragColor;

in vec3 Normal;  
in vec3 FragPos;
in vec3 ourColor;  
  
uniform vec3 cameraPos; 
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
vec3 CalcDirectionalLight(vec3 lightDir, vec3 normal, vec3 viewDir);
float CalcAttenuation(PointLight light, vec3 fragPos);

void main()
{
    vec3 FragOut = vec3(0.0, 0.0, 0.0);
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(cameraPos - FragPos);
    if(numLights > 0)
    {
        for(int i = 0; i < numLights; i++){
            FragOut += CalcPointLight(pointLights[i], norm, FragPos, viewDir);
        }
    }

    //FragOut += CalcDirectionalLight(normalize(directionalLightDir), norm, viewDir);
    FragOut = FragOut * ourColor;
    FragColor = vec4(FragOut, 1.0);

}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)

{
        //ambient
    vec3 ambient = light.ambient;
  	
    // diffuse 
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * light.diffuse;
    
    // specular
    vec3 reflectDir = reflect(-lightDir, normal);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = light.specular * spec;  

    //attenuation
    float attenuation = CalcAttenuation(light, fragPos);
    diffuse *= attenuation;
    specular *= attenuation;
    ambient *= attenuation;

    return (ambient + diffuse + specular);
}

// vec3 CalcDirectionalLight(vec3 lightDir, vec3 normal, vec3 viewDir)
// {
//     //ambient
//     vec3 ambient = ambientStrength * lightColor;
//     // diffuse
//     float diff = max(dot(normal, lightDir), 0.0);
//     vec3 diffuse = diff * lightColor;
//     // specular
//     vec3 reflectDir = reflect(-lightDir, normal);  
//     float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
//     vec3 specular = specularStrength * spec * lightColor;  
//     return (ambient + diffuse + specular);
// }


const float constant = 1.0;
const float linear = 0.09;
const float quadratic = 0.032;

float CalcAttenuation(PointLight light, vec3 fragPos)
{   

    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (1.0 + linear * distance + quadratic * (distance * distance));
    return attenuation;
}
