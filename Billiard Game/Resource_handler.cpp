#include "Resource_handler.hpp"

Resource_handler::Resource_handler()
{
	models = NULL;
	programs = NULL;
}


Resource_handler::~Resource_handler()
{
	unload();
}


// function to load resources before drawing
void Resource_handler::load(const char * path_resource_file)
{
	FILE * input_file = fopen(path_resource_file, "r");

	if (input_file == NULL){
		cerr << "load file " << path_resource_file << " failed!!!";
	}

	//Reading model
	fscanf(input_file, "#Models: %d\n", &nb_Models);
	models = new Model3D[nb_Models];

	for (int model_ID = 0; model_ID < nb_Models; ++model_ID){
		int temp;
		fscanf(input_file, "ID %d\n", &temp);
		char model_PATH[300];
		fscanf(input_file, "FILE \"%[^\"]\"\n", model_PATH);
		//cout << "File name: " << model_PATH << endl;
		models[model_ID].load_Model(model_PATH);
	}

	//Reading shaders
	fscanf(input_file, "#Shaders: %d\n", &nb_Programs);
	programs = new Program[nb_Programs];
	for (int shader_ID = 0; shader_ID < nb_Programs; ++shader_ID){
		int temp;
		fscanf(input_file, "ID %d\n", &temp);
		char vertex_Shader[300], fragment_Shader[300];
		fscanf(input_file, "VS \"%[^\"]\"\n", vertex_Shader);
		fscanf(input_file, "FS \"%[^\"]\"\n", fragment_Shader);
		cerr << "Shader = " << vertex_Shader << " " << fragment_Shader;
		programs[shader_ID].attach_Shader(new Shader(vertex_Shader, GL_VERTEX_SHADER));
		programs[shader_ID].attach_Shader(new Shader(fragment_Shader, GL_FRAGMENT_SHADER));
	}
	fclose(input_file);
}


// Function to unload (clear memory spent for) resources
void Resource_handler::unload()
{
	if (models != NULL){
		delete[] models; 
		models = NULL;
	}
	if (programs != NULL){
		delete[] programs;
		programs = NULL;
	}
}


// Get the id model from list models
Model3D * Resource_handler::getModel(int model_ID)
{
	return &(models[model_ID]);
}

// Get the id shader program from list programs
Program * Resource_handler::getProgram(int program_ID)
{
	return &(programs[program_ID]);
}