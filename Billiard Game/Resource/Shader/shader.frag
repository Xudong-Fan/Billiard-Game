#version 330

layout (std140) uniform Material {
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    float shininess;
    vec4 emissive;
} material;

layout (std140) uniform Light{
    vec4 ambientIntensity;
    vec4 diffuseIntensity;
    vec4 specularIntensity;
    vec3 position;		//In world coordinates
	
	vec3 spotDirection;
    float spotExponent;
    float spotCutoff;

    float attenuationConstant;
    float attenuationLinear;
    float attenuationQuadratic;
} light;

uniform vec3 view_Pos; //Camera position in world coordinates
uniform int tex_Count;
uniform sampler2D u_Sampler;

in vec3 Position; //In world coordinates
in vec2 Tex_Coord;
in vec3 Normal; //In world coordinates

void main(){
	//Some basic things to active uniform
	vec3 q = light.spotDirection;
    float w = light.spotExponent;
    float r = light.spotCutoff;

    vec3 lightDir = normalize(light.position - Position);
    vec3 viewDir = normalize(view_Pos - Position);
    vec3 normalDir = normalize(Normal);

	//Ambient
    vec4 I_a = light.ambientIntensity * material.ambient;

    //Diffuse reflection
	float diffuseCoef = max(dot(normalDir, lightDir), 0.0);
	vec4 I_d = light.diffuseIntensity * material.diffuse * diffuseCoef;

    //Specular reflection
	vec3 reflectDir = normalize(reflect(-lightDir, normalDir));
	float specularCoef = max(dot(reflectDir, viewDir), 0.0);
	specularCoef = pow(specularCoef, material.shininess);
    vec4 I_s = light.specularIntensity * material.specular * specularCoef;

	//Attenuation
	float distance_2_Light = distance(light.position, Position);
	float attenuation = light.attenuationConstant;
	attenuation += light.attenuationLinear * distance_2_Light;
	attenuation += light.attenuationQuadratic * pow(distance_2_Light, 2);
	attenuation = 1.0 /attenuation;

	//vec4 linearColor = attenuation * (I_a + I_d + I_s) + material.emissive;
	vec4 linearColor = I_a + I_d + I_s;
	vec4 texColor = vec4(1.0, 1.0, 1.0, 1.0);
	if (tex_Count != 0){
		texColor = texture(u_Sampler, Tex_Coord);
	}

	//Gamma correction
	vec4 gamma = vec4(1.0/2.2);
	gl_FragColor = pow(linearColor, gamma) * texColor;
}
