#pragma once
#include "Resource_handler.hpp"

#include "Scene.hpp"

class Scene_handler
{
public:
	Scene_handler();
	~Scene_handler();
public:
	// Load all the scenes in the scene list
	void load(const char * path_Scene_List, Resource_handler * resource_Handler);
	// Render the scene with id
	void renderScene(int sceneId);
	// Delete old data
	void unload();
	Scene * getScene(int sceneId);
private:
	Scene * scenes;
	//Scene * scenes;
	int nb_Scenes;
};

