#include "Scene_handler.hpp"

#include "Resource_handler.hpp"


Scene_handler::Scene_handler()
{
	scenes = NULL;
}


Scene_handler::~Scene_handler()
{
	unload();
}


// Load the list of scene and then load all the scenes
void Scene_handler::load(const char * path_Scene_List, Resource_handler * resource_Handler)
{
	unload();
	FILE * input_file = fopen(path_Scene_List, "r");

	if (input_file == NULL) {
		cerr << "load file " << path_Scene_List << " failed!!!";
	}

	//Reading object
	fscanf(input_file, "#Scenes: %d\n", &nb_Scenes);
	scenes = new Scene[nb_Scenes];
	for (int Scene_id = 0; Scene_id < nb_Scenes; ++Scene_id) {
		int temp;
		fscanf(input_file, "ID %d\n", &temp);
		char file_Scene[300];
		fscanf(input_file, "SCENE \"%[^\"]\"\n", file_Scene);
		//cout << fileScene;
		scenes[Scene_id].load(file_Scene, resource_Handler);
	}
	fclose(input_file);
}


// Render the scene with id
void Scene_handler::renderScene(int sceneId)
{
	scenes[sceneId].render();
}


// Delete old data
void Scene_handler::unload()
{
	if (scenes != NULL) {
		delete[] scenes;
		scenes = NULL;
	}
}

Scene * Scene_handler::getScene(int sceneId) {
	return &(scenes[sceneId]);
}
