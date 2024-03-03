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

uniform int hasMaterial;

uniform int hasTexture;

const float screenGamma = 2.2; // Assume the monitor is calibrated to the sRGB color space

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

struct Material {
	sampler2D diffuseTexture;
	sampler2D specularTexture;
	float     shininess;
	vec3      baseColor;
    int 	 hasDiffuseTexture;
    int 	 hasSpecularTexture;
};

uniform PointLight pointLights[16];
uniform Material material;

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);

    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (distance * distance);
    // combine results
    vec3 ambient  = light.ambient  * vec3(texture(material.diffuseTexture, texCoord)) * material.baseColor;
    vec3 diffuse  = light.diffuse * diff * vec3(texture(material.diffuseTexture, texCoord)) * material.baseColor;

    if (material.hasDiffuseTexture == 0) 
    {
        diffuse  = light.diffuse * diff * material.baseColor;
        ambient = light.ambient  * material.baseColor;
    }

    vec3 specular = light.specular * spec * vec3(texture(material.specularTexture, texCoord));
    if (material.hasSpecularTexture == 0) 
	{
	    specular = light.specular * spec;
	}

     ambient  *= attenuation;
    // diffuse  *= attenuation;
     specular *= attenuation;
    return (diffuse);
} 

// https://en.wikipedia.org/wiki/Blinn–Phong_reflection_model
void main()
{
   // properties
    vec3 norm = normalize(worldNormal);
    vec3 viewDir = normalize(-worldPos);

    // phase 1: Directional lighting
    vec3 result = vec3(0,0,0); //CalcDirLight(dirLight, norm, viewDir);
    // phase 2: Point lights

    for(int i = 0; i < numLights; i++)
       result += CalcPointLight(pointLights[i], norm, worldPos, viewDir);    

    if (enableFog == 1)
    {
        float fogAmount = (length(worldPos - viewPos) - startFogDistance) / (endFogDistance - startFogDistance);
        if (fogAmount > 1.0)
		    fogAmount = 1.0;
    
     //   result = mix(result, backgroundColor, fogAmount);
    }

    // phase 3: Spot light
    //result += CalcSpotLight(spotLight, norm, FragPos, viewDir);    
    
    fragmentColor = vec4(result, 1.0);
}

