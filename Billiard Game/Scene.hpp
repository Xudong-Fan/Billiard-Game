#pragma once

#include "Resource_handler.hpp"
#include "Object.hpp"
#include "Ball.hpp"

class Scene
{
public:
	Scene();
	~Scene();
	// Load configure information from "scene.txt" and create objects in "resource.txt" 
	void load(char * path_scene_file, Resource_handler * resource_handler);
	// render 3D scene
	void render();
	// delete old data
	void unload();

	Camera * get_Using_Camera();
	void set_Camera(int camera_Id);
	
	Object * * objects;

private:
	Camera * cameras;
	int nb_Cameras;
	int camera_to_use_ID;

	Light * lights;
	int nb_Lights;

	//Object * * objects;
	int nb_Objects;

	GLdouble time_Last_Frame;
};

