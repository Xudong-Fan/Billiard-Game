#pragma once
#include "Resource_handler.hpp"
#include "Model.hpp"
#include "Shader.hpp"
#include "Camera.hpp"
#include "Light.hpp"
#include "Constant.hpp"

class Object
{
public:
	Object();
	~Object();
	// render the 3D object
	virtual void render(Camera * camera, Light * light, GLdouble elapsedTime);
	// load Model3D from resource_Handler
	void load_Model(int modelId, Resource_handler * resource_Handler);
	// load Program from resource_Handler
	void load_Program(int programId, Resource_handler * resource_Handler);
	void unload();

	void translate(float x, float y, float z);
	void translate(glm::vec3 translateVector);
	void rotate(float angle, float x, float y, float z);
	void rotate(float angle, glm::vec3 rotateVector);
	void scale(float x);

	glm::mat4 getModelMatrix() const;
private:
	Model3D * model;
	Program * program;

	glm::mat4 modelMatrix;
};

