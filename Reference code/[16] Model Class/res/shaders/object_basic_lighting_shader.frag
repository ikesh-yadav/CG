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
in vec2 TexCoord;


uniform vec3 viewPos;
uniform vec3 objectColor;
uniform Light light; 
uniform Material material;
uniform bool texture_enable;

// Texture samplers
uniform sampler2D u_texture;

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

    vec3 result;

    //vec3 result = (ambient + diffuse + specular) * vec3( 0.3f );
    if (texture_enable){
        result = (ambient + diffuse + specular);//* vec4(result, 1.0f);
        color =  texture(u_texture, TexCoord); 
    } else {
        result = (ambient + diffuse + specular) * material.color;
        color = vec4(result, 1.0f);
    }
}