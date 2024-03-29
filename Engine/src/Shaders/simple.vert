#version 420

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_color;
layout(location = 2) in vec3 in_normal;
layout(location = 3) in vec2 in_texCoord;

uniform mat4 normalMatrix;
uniform mat4 modelViewProjectionMatrix;
uniform mat4 modelViewMatrix;
uniform mat4 modelMatrix;


out vec3 worldPos;
out vec3 worldNormal;
out vec4 color;
out vec2 texCoord;

void main()
{
	color = vec4(in_color, 0);
	texCoord = in_texCoord;

	gl_Position = (modelViewProjectionMatrix* vec4(in_position, 1.0));
    worldPos = vec3(modelMatrix * vec4(in_position, 1.0));
	worldNormal = (normalMatrix * vec4(in_normal, 0.0)).xyz;
}
