#pragma once
#include "Model.hpp"
#include "Shader.hpp"

#include <iostream>

using namespace std;

class Resource_handler
{
public:
	Resource_handler();
	~Resource_handler();

	// Function to load resources
	void load(const char * path_resource_file);

	// Function to clear memory spent for resources
	void unload();

	// Get the model ID from the model list
	Model3D * getModel(int model_Id);

	// Get the shader program ID from programs list
	Program * getProgram(int program_Id);


private:
	Model3D * models;
	Program * programs;

	int nb_Models;
	int nb_Programs;
};

