#include "Scene.hpp"

Scene::Scene()
{
	objects = NULL;
	nb_Objects = 0;

	cameras = NULL;
	nb_Cameras = 0;

	lights = NULL;
	nb_Lights = 0;

	time_Last_Frame = 0.0f;
}


Scene::~Scene()
{
	unload();
}


// Load configure information from the scene file and create objects with resource from resource_handler
void Scene::load(char * path_scene_file, Resource_handler * resource_handler)
{
	unload();
	FILE * input_file = fopen(path_scene_file, "r");

	if (input_file == NULL){
		cerr << "load file " << path_scene_file << " failed!!!";
	}

	//Read object
	fscanf(input_file, "#Objects: %d\n", &nb_Objects);
	cout << "Total number of objects = " << nb_Objects << endl;
	objects = new Object * [nb_Objects];
	for (int object_ID = 0; object_ID < nb_Objects; ++object_ID){
		int temp;
		fscanf(input_file, "ID %d\n", &temp);
		char object_Type[300];
		fscanf(input_file, "OBJECT_TYPE %s\n", object_Type);
		cout << "OBJECT_TYPE = " << object_Type << endl;
		if (strcmp(object_Type, "BALL") == 0){
			objects[object_ID] = new Ball();
		}
		else {
			objects[object_ID] = new Object();
		}
		//Model
		int model_ID;
		fscanf(input_file, "MODEL %d\n", &model_ID);
		//cout << "MODEL = " << model_ID << endl;
		objects[object_ID]->load_Model(model_ID, resource_handler);		
		//Shaders
		int shader_ID;
		fscanf(input_file, "SHADER %d\n", &shader_ID);
		//cout << "SHADER = " << shader_ID << endl;
		objects[object_ID]->load_Program(shader_ID, resource_handler);
		//Import position, rotation, scale of the objects
		float scale_value;
		fscanf(input_file, "SCALE %f\n", &scale_value);
		objects[object_ID]->scale(scale_value);
		float x, y, z;
		fscanf(input_file, "POSITION %f, %f, %f\n", &x, &y, &z);
		objects[object_ID]->translate(x, y, z);
		fscanf(input_file, "ROTATION %f, %f, %f\n", &x, &y, &z);


	}

	//LIGHTS
	fscanf(input_file, "#LIGHTS\n");
	float x, y, z;
	fscanf(input_file, "AmbientColor %f, %f, %f\n", &x, &y, &z);
	fscanf(input_file, "AmbientWeight %f\n", &x);
	int nb_Lights;
	fscanf(input_file, "LightsCount %d\n", &nb_Lights);
	lights = new Light[nb_Lights];
	for (int idLight = 0; idLight < nb_Lights; ++idLight){
		int temp;
		fscanf(input_file, "ID %d\n", &temp);
		fscanf(input_file, "POS/DIR %f, %f, %f\n", &x, &y, &z);
		LightSource * lightSource = new LightSource();
		lightSource->position = glm::vec3(x, y, z);
		lightSource->ambientIntensity = glm::vec4(0.25f, 0.25f, 0.25f, 1.0f);
		lightSource->diffuseIntensity = glm::vec4(0.35f, 0.35f, 0.35f, 1.0f);
		lightSource->specularIntensity = glm::vec4(0.75f, 0.75f, 0.75f, 1.0f);
		lights[idLight].setLightSource(lightSource);
		char type[10];
		fscanf(input_file, "TYPE %s\n", type);
		fscanf(input_file, "COLOR %f, %f, %f\n", &x, &y, &z);
		char moving[10];
		fscanf(input_file, "MOVING %s\n", moving);
		float radius;
		fscanf(input_file, "RADIUS %f\n", &radius);
		float speed;
		fscanf(input_file, "SPEED %f\n", &speed);
	}

	//CAMERAs
	fscanf(input_file, "#Camera: %d\n", &nb_Cameras);
	cameras = new Camera[nb_Cameras];
	for (int camera_ID = 0; camera_ID < nb_Cameras; ++camera_ID){
		int temp;
		fscanf(input_file, "ID %d\n", &temp);
		fscanf(input_file, "POSITION %f, %f, %f\n", &x, &y, &z);		
		cameras[camera_ID].translate(glm::vec3(x, y, z), WORLD_COORDINATES, WORLD_COORDINATES);
		float angle;
		fscanf(input_file, "ROTATION %f, %f, %f, %f\n", &angle, &x, &y, &z);
		cameras[camera_ID].rotate(angle, glm::vec3(x, y, z), VIEW_COORDINATES);
		//cout << "Camera position = " << x << " " << y << " " << z << endl;

		GLfloat zNear, zFar, fovy, aspect;
		fscanf(input_file, "NEAR %f\n", &zNear);
		fscanf(input_file, "FAR %f\n", &zFar);
		fscanf(input_file, "FOV %f\n", &fovy);
		fscanf(input_file, "ASPECT %f\n", &aspect);
		cameras[camera_ID].setProjectionMatrix(glm::perspective(fovy, aspect, zNear, zFar));
		//cout << fovy << " " << aspect  << " " << zNear << " " << zFar << endl;
		cameras[camera_ID].push();
	}
	
	set_Camera(0);
	fclose(input_file);
}

// render 3D scene
void Scene::render()
{
	GLdouble elapsedTime = glutGet(GLUT_ELAPSED_TIME) - time_Last_Frame;

	Program::useProgram(NULL);

	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(glm::value_ptr(get_Using_Camera()->getProjectionMatrix()));
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(glm::value_ptr(get_Using_Camera()->getViewMatrix()));

	for (int object_Id = 0; object_Id < nb_Objects; ++object_Id){
		objects[object_Id]->render(get_Using_Camera(), &lights[0], elapsedTime);
		if (elapsedTime > Constant::TIME_FOR_A_FRAME){
			if (object_Id > 1){
				Ball * ball = (Ball *)(this->objects[object_Id]);
				ball->enter_Pocket(object_Id);
				ball->collision_Table();
				for (int otherobject_Id = object_Id + 1; otherobject_Id < nb_Objects; ++otherobject_Id){
					//cout << "Collision test: " << object_Id << " collides with " << otherobject_Id << endl;
					Ball * otherBall = (Ball *)(this->objects[otherobject_Id]);
					ball->collision_Balls(otherBall);
				}
			}
			time_Last_Frame = glutGet(GLUT_ELAPSED_TIME);
		}
	}

	if (elapsedTime > Constant::TIME_FOR_A_FRAME){
		time_Last_Frame = glutGet(GLUT_ELAPSED_TIME);
	}
}

// Delete old data
void Scene::unload()
{
	if (objects != NULL){
		for (int object_Id = 0; object_Id < nb_Objects; ++object_Id){
			objects[object_Id]->unload();
			if (objects[object_Id] != NULL){
				delete objects[object_Id];
				objects[object_Id] = NULL;
			}
		}
		delete[] objects;
		objects = NULL;
		nb_Objects = 0;
	}
	if (cameras != NULL){
		delete[] cameras;
		cameras = NULL;
		nb_Cameras = 0;
	}
	if (lights != NULL){
		delete[] lights;
		lights = NULL;
		nb_Lights = 0;
	}
}

Camera * Scene::get_Using_Camera(){
	if (cameras == NULL || camera_to_use_ID >= nb_Cameras){
		cerr << "Get unknown camera " << endl;
		return NULL;
	}
	return &(cameras[camera_to_use_ID]);
}

void Scene::set_Camera(int camera_Id){
	if (camera_Id < nb_Cameras && camera_Id >= 0){
		cout << "Camera " << camera_Id << " is using right now." << endl;
		this->camera_to_use_ID = camera_Id;
	}
	else {
		cerr << "set using unknown camera " << endl;
	}
}



