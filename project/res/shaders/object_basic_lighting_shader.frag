#version 330 core
struct Light {
    vec3 position;
    vec3 color;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;

    vec3 color;
}; 
  
out vec4 color;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;
flat in int TexID;


uniform vec3 viewPos;
uniform vec3 objectColor;
uniform Light light; 
uniform Material material;
uniform bool texture_enable;

// Texture samplers
uniform sampler2D u_texture0;
uniform sampler2D u_texture1;

uniform sampler2D texture_diffuse0;
uniform sampler2D texture_specular0;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;

uniform bool isModel;

void main()
{

    // ambient
    vec3 ambient = light.color * material.ambient;
  	
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.color * (diff * material.diffuse);
    
    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.color * (spec * material.specular);  

    vec3 result = (ambient + diffuse + specular);

    if (isModel) {
            color = vec4(texture( texture_diffuse0, TexCoords )) * vec4(result, 0.7f);
    }else {
        if (texture_enable){
            if(TexID == 1) color = vec4(texture(u_texture0, TexCoords)) * vec4(result, 1.0f);
            else color = vec4(texture(u_texture1, TexCoords)) * vec4(result, 1.0f);        
        } else {
            color =  vec4(result * material.color, 1.0f); 
        }
    }
    

}