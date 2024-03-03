#version 420 

// required by GLSL spec Sect 4.5.3 (though nvidia does not, amd does)
precision highp float;

layout(location = 0) out vec4 fragmentColor;

uniform layout(binding = 0) sampler2D testTexture;

uniform vec3 viewPos;

uniform vec3 backgroundColor;

uniform int enableFog;

uniform float startFogDistance;
uniform float endFogDistance;

uniform int numLights;
uniform int numSpotLights;

uniform int hasMaterial;

uniform int hasTexture;

in vec3 worldPos;
in vec3 worldNormal;
in vec4 color;
in vec2 texCoord;

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
	float cutOff;
	float outerCutOff;

	float constant;
	float linear;
	float quadratic;  

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct Material {
	sampler2D diffuseTexture;
	sampler2D specularTexture;
	float     shininess;
	vec3      baseColor;
    int 	 hasDiffuseTexture;
    int 	 hasSpecularTexture;
};

uniform PointLight pointLights[16];
uniform SpotLight spotLights[16];
uniform Material material;

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 baseColor = vec3(0,0,0);
	if (material.hasDiffuseTexture == 1) 
	{
		baseColor = vec3(texture(material.diffuseTexture, texCoord)) * material.baseColor;
	}
	else
	{
		baseColor = material.baseColor;
	}

			float distance = length(light.position - fragPos);
		float attenuation = 1.0 / (light.constant + light.linear * distance + 
  					 light.quadratic * (distance * distance));    

    vec3 lightDir = normalize(light.position - fragPos);
	float theta = dot(lightDir, normalize(-light.direction));

	vec3 ambient = 0.2 * baseColor;
  

	// diffuse
	float diff = max(dot(lightDir, normal), 0.0);
	vec3 diffuse = diff * baseColor;
	// specular
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = 0.0;

	vec3 halfwayDir = normalize(lightDir + viewDir);  
	spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);

    vec3 specular = vec3(0.3) * spec; // assuming bright white light color

	float epsilon   = light.cutOff - light.outerCutOff;
	float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);  
	diffuse *= intensity;
	specular *= intensity;

	ambient *= attenuation;  
//	diffuse *= attenuation;
//	specular *= attenuation;
	return diffuse + specular + ambient;
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 baseColor = vec3(0,0,0);
    if (material.hasDiffuseTexture == 1) 
	{
		baseColor = vec3(texture(material.diffuseTexture, texCoord)) * material.baseColor;
	}
	else
	{
		baseColor = material.baseColor;
	}

    // ambient
    vec3 ambient = 0.2 * baseColor;
    // diffuse
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * baseColor;
    // specular
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = 0.0;

    vec3 halfwayDir = normalize(lightDir + viewDir);  
    spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);

    vec3 specular = vec3(0.3) * spec; // assuming bright white light color

    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
  			     light.quadratic * (distance * distance));    

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return ambient + diffuse + specular;
} 

// https://en.wikipedia.org/wiki/Blinn–Phong_reflection_model
void main()
{
   // properties
    vec3 norm = normalize(worldNormal);
    vec3 viewDir = normalize(viewPos - worldPos);

    // phase 1: Directional lighting
    vec3 result = vec3(0,0,0); //CalcDirLight(dirLight, norm, viewDir);
    // phase 2: Point lights

  //  for(int i = 0; i < numLights; i++)
  //     result += CalcPointLight(pointLights[i], norm, worldPos, viewDir);    

	for (int i = 0; i < numSpotLights; i++)
		result += CalcSpotLight(spotLights[i], norm, worldPos, viewDir);

    if (enableFog == 1)
    {
        float fogAmount = (length(worldPos - viewPos) - startFogDistance) / (endFogDistance - startFogDistance);
        if (fogAmount > 1.0)
		    fogAmount = 1.0;
    
        result = mix(result, backgroundColor, fogAmount);
    }

    // phase 3: Spot light
    //result += CalcSpotLight(spotLight, norm, FragPos, viewDir);    
    
    fragmentColor = vec4(result, 1.0);
}

