#version 330

uniform	mat4 projMatrix;
uniform	mat4 viewMatrix;
uniform	mat4 modelMatrix;

layout (location = 0) in vec3 position;	//in model coordinates
layout (location = 1) in vec2 tex_Coord;
layout (location = 2) in vec3 normal;	//in model coordinates

out vec3 Position;	//in world coordiantes
out vec2 Tex_Coord;
out vec3 Normal;	//in world coordiantes

void main(){
	mat3 normalMatrix = transpose(inverse(mat3(modelMatrix)));
	Position = vec3(modelMatrix * vec4(position, 1.0));
    Tex_Coord = tex_Coord;
    Normal = normalMatrix * normal;

	gl_Position = projMatrix * viewMatrix * modelMatrix * vec4(position, 1.0);
}