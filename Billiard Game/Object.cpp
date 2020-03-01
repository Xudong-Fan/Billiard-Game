#include "Object.hpp"


Object::Object()
{
}


Object::~Object()
{
}


// render the 3D object
void Object::render(Camera * camera, Light * light, GLdouble elapsedTime)
{
	Program::useProgram(program);

	GLuint p = program->getProgram();

	Mesh::aPositionLoc = glGetAttribLocation(p, "position");
	Mesh::aTex_CoordLoc = glGetAttribLocation(p, "Tex_Coord");
	Mesh::aNormalLoc = glGetAttribLocation(p, "normal");

	Node::uModelMatrixLoc = glGetUniformLocation(p, "modelMatrix");
	Camera::uViewMatrixLoc = glGetUniformLocation(p, "viewMatrix");
	Camera::uProjMatrixLoc = glGetUniformLocation(p, "projMatrix");

	Material::uboMaterialLoc = glGetUniformBlockIndex(p, "Material");
	Light::uboLightLoc = glGetUniformBlockIndex(p, "Light");
	Texture::uTextureCountLoc = glGetUniformLocation(p, "tex_Count");
	Camera::uview_PosLoc = glGetUniformLocation(p, "view_Pos");

	glUniformMatrix4fv(Camera::uViewMatrixLoc, 1, GL_FALSE, glm::value_ptr(camera->getViewMatrix()));
	glUniformMatrix4fv(Camera::uProjMatrixLoc, 1, GL_FALSE, glm::value_ptr(camera->getProjectionMatrix()));

	glm::vec3 view_Pos = camera->getViewPoint();
	glUniform3f(Camera::uview_PosLoc, view_Pos.x, view_Pos.y, view_Pos.z);
#ifdef PRINT_CAMERA_POSITION
	cout << "Camera Pos:\t" << view_Pos << endl;
#endif

	if (light != NULL){
		glBindBufferRange(GL_UNIFORM_BUFFER, Light::uboLightLoc,
			light->getUBOLight(), 0, sizeof(LightMaterial));
#ifdef PRINT_LIGHT_SOURCE
		cout << *(light->uLight) << endl;
#endif
	}

	model->render();
}


// load Model3D from resource_Handler
void Object::load_Model(int modelId, Resource_handler * resource_Handler)
{
	model = resource_Handler->getModel(modelId);
}


// load Program from resource_Handler
void Object::load_Program(int programId, Resource_handler * resource_Handler)
{
	program = resource_Handler->getProgram(programId);

	Program::useProgram(program);

}


void Object::unload()
{
}

void Object::translate(float x, float y, float z){
	model->translate(glm::vec3(x, y, z), WORLD_COORDINATES, WORLD_COORDINATES);
	modelMatrix = glm::translate(glm::vec3(x, y, z)) * modelMatrix;
}

void Object::translate(glm::vec3 translateVector){
	model->translate(translateVector, WORLD_COORDINATES, WORLD_COORDINATES);
	modelMatrix = glm::translate(translateVector) * modelMatrix;
}

void Object::rotate(float angle, float x, float y, float z){
	model->rotate(angle, glm::vec3(x, y, z), MODEL_COORINATES);
	modelMatrix = modelMatrix * glm::rotate(angle, glm::vec3(x, y, z));
}

void Object::rotate(float angle, glm::vec3 rotateVector){
	model->rotate(angle, rotateVector, MODEL_COORINATES);
	modelMatrix = modelMatrix * glm::rotate(angle, rotateVector);
}


void Object::scale(float scaleVal){
	model->scale(scaleVal, MODEL_COORINATES);
	modelMatrix = modelMatrix * glm::scale(glm::vec3(scaleVal));
}

glm::mat4 Object::getModelMatrix() const{
	//return modelMatrix;
	return model->getModelMatrix();
}