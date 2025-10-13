#version 330 core
struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

out vec4 color;

uniform vec3 viewPos;
uniform Material material;

// Sol (día)
uniform Light light;
// Luna (noche)
uniform Light newlight;

uniform sampler2D texture_diffusse;

vec3 CalcLight(Light L, vec3 norm, vec3 fragPos, vec3 viewDir)
{
    // Ambient 
    vec3 ambient  = L.ambient * material.diffuse;

    // Diffuse
    vec3 lightDir = normalize(L.position - fragPos);
    float diff    = max(dot(norm, lightDir), 0.0);
    vec3 diffuse  = L.diffuse * diff * material.diffuse;

    // Specular (Phong)
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec      = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular   = L.specular * (spec * material.specular);

    return ambient + diffuse + specular;
}

void main()
{
    vec3 norm    = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    // Sumar Sol + Luna. 
    // la contribución se apaga sola.
    vec3 result = vec3(0.0);
    result += CalcLight(light,    norm, FragPos, viewDir);   // Sol
    result += CalcLight(newlight, norm, FragPos, viewDir);   // Luna

    // Aplica tu textura
    vec4 tex = texture(texture_diffusse, TexCoords);
    color = vec4(result, 1.0) * tex;
}
