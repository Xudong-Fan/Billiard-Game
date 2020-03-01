#include "Play_conf.hpp"


float Keyboard::coef = 0.005f;
Camera * Keyboard::cam = NULL;
Scene_handler * Keyboard::scene_Handler = NULL;
Camera* Mouse::cam = NULL;
glm::ivec2 Mouse::drag_Start_Pos = glm::ivec2(-1);
int Mouse::window_Width = 1000, Mouse::window_Height = 800;
const float Mouse::coef = 90.0f;

bool Mouse::R_BUTTON_DOWN = false;
bool Mouse::L_BUTTON_DOWN = false;
int Mouse::start_left_X = 0;
int Mouse::start_left_Y = 0;
Ball * Mouse::ball = NULL;

void Keyboard::set_Camera(Camera* cam) {
	Keyboard::cam = cam;
}

void Keyboard::Set_scene_handler(Scene_handler * new_Scene_handler) {
	Keyboard::scene_Handler = new_Scene_handler;
}

void Keyboard::special_Func(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_UP:
		if (glutGetModifiers() == GLUT_ACTIVE_CTRL) { //Move camera frontward
			cam->translate(0.0f, 0.0f, -coef * 2, VIEW_COORDINATES, VIEW_COORDINATES);
		}
		else { //Move Camera up
			cam->translate(0.0f, coef, 0.0f, VIEW_COORDINATES, VIEW_COORDINATES);
		}
		break;
	case GLUT_KEY_DOWN:
		if (glutGetModifiers() == GLUT_ACTIVE_CTRL) { //Move camera backward
			cam->translate(0.0f, 0.0f, coef * 2, VIEW_COORDINATES, VIEW_COORDINATES);
		}
		else { //Move Camera down
			cam->translate(0.0f, -coef, 0.0f, VIEW_COORDINATES, VIEW_COORDINATES);
		}
		break;
	case GLUT_KEY_RIGHT: //Move Camera right
		cam->translate(coef, 0.0f, 0.0f, VIEW_COORDINATES, VIEW_COORDINATES);
		break;
	case GLUT_KEY_LEFT: //Move Camera left
		cam->translate(-coef, 0.0f, 0.0f, VIEW_COORDINATES, VIEW_COORDINATES);
		break;
	case GLUT_KEY_F4:
		if (glutGetModifiers() == GLUT_ACTIVE_ALT) { //Exit
			exit(EXIT_SUCCESS);
		}
	default:
		return;
	}
	glutPostRedisplay();
}

void Keyboard::key_board_Func(unsigned char key, int x, int y) {
	//Do something here
	switch (key) {
	case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
		Keyboard::scene_Handler->getScene(0)->set_Camera(key - '0');
		Keyboard::cam = Keyboard::scene_Handler->getScene(0)->get_Using_Camera();
		Mouse::set_Camera(Keyboard::cam);
		break;
	default:
		return;
	}
	glutPostRedisplay();
}

void Mouse::set_Camera(Camera* cam) {
	Mouse::cam = cam;
}

void Mouse::set_Window(int width, int height) {
	Mouse::window_Width = width;
	Mouse::window_Height = height;
}

void Mouse::mouse_Func(int button, int state, int x, int y) {
	switch (button) {
	    case GLUT_RIGHT_BUTTON:
		    if (state == GLUT_DOWN) { //Right button is press
			    drag_Start_Pos = glm::ivec2(x, y);
			    cam->push();
			    R_BUTTON_DOWN = true;
		    }
		    else { //Left Mouse is release
		    	R_BUTTON_DOWN = false;
			    cam->pop();
			    apply_Change(glm::ivec2(x, y));

			    drag_Start_Pos = glm::ivec2(-1);
			    glutPostRedisplay();
		    }
		    break;
	    case GLUT_LEFT_BUTTON:
		    if (state == GLUT_DOWN) {
			    L_BUTTON_DOWN = true;
			    start_left_X = x;
			    start_left_Y = y;
		    }
		    else {//Left button is release
			    L_BUTTON_DOWN = false;

			    glm::vec4 cameraInWorld = glm::inverse(cam->getViewMatrix()) * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

			    glm::vec4 firstRay = getRay(start_left_X, start_left_Y);
			    firstRay.x = firstRay.x / firstRay.y * -cameraInWorld.y;
			    firstRay.z = firstRay.z / firstRay.y * -cameraInWorld.y;
			    firstRay.y = -cameraInWorld.y;
			    //cout << "firstRay = " << firstRay.x << " " << firstRay.y << " " << firstRay.z << " " << firstRay.w << " " << endl;

			    glm::vec4 secondRay = getRay(x, y);
			    secondRay.x = secondRay.x / secondRay.y * -cameraInWorld.y;
			    secondRay.z = secondRay.z / secondRay.y * -cameraInWorld.y;
			    secondRay.y = -cameraInWorld.y;
			    //cout << "secondRay = " << secondRay.x << " " << secondRay.y << " " << secondRay.z << " " << secondRay.w << " " << endl;

			    glm::vec3 velocity = glm::vec3(secondRay.x - firstRay.x, secondRay.y - firstRay.y, secondRay.z - firstRay.z);
			    if (glm::length(velocity) > 0) {
				    velocity = glm::normalize(velocity);
			    	//cout << "velocity = " << velocity.x << " " << velocity.y << " " << velocity.z << " " << endl;
			    	//cout << "length = " << glm::length(velocity) << endl;
			    	// Here, I need to add a function for setting different speed. 
				    ball->set_Velocity(glm::vec3(velocity.x * 0.01f, 0.0f, velocity.z * 0.01f));
			    }
		    }
		break;
	}
}

glm::vec4 Mouse::getRay(float mouse_x, float mouse_y) {
	float x = (2.0f * mouse_x) / Constant::SCREEN_WIDTH - 1.0f;
	float y = 1.0f - (2.0f * mouse_y) / Constant::SCREEN_HEIGHT;
	float z = 1.0f;

	glm::vec3 ray_nds = glm::vec3(x, y, z);
	glm::vec4 ray_clip = glm::vec4(ray_nds.x, ray_nds.y, -1.0f, 1.0f);
	glm::vec4 ray_eye = glm::inverse(cam->getProjectionMatrix()) * ray_clip;

	ray_eye = glm::vec4(ray_eye.x, ray_eye.y, -1.0, 0.0);
	glm::vec4 ray_world4 = glm::inverse(cam->getViewMatrix()) * ray_eye;
	glm::vec3 result = glm::vec3(ray_world4.x, ray_world4.y, ray_world4.z);

	// Don't forget to normalise the vector
	result = glm::normalize(result);
	return glm::vec4(result.x, result.y, result.z, 0.0f);
}

void Mouse::mouse_Motion_Func(int x, int y) {
	if (R_BUTTON_DOWN) {
		cam->pop(); //Remove old matrix
		cam->push();
		apply_Change(glm::ivec2(x, y));
		glutPostRedisplay();
	}
}

void Mouse::apply_Change(const glm::ivec2& curPos) {
	glm::ivec2 delta = drag_Start_Pos - curPos;

	glm::mat3 viewInverse = glm::inverse(glm::mat3(cam->getViewMatrix()));
	glm::vec3 vAxis = glm::normalize(viewInverse * yAxis);
	cam->rotate(coef * delta.x / float(window_Width), vAxis, WORLD_COORDINATES);

	viewInverse = glm::inverse(glm::mat3(cam->getViewMatrix()));
	glm::vec3 uAxis = glm::normalize(viewInverse * xAxis);
	cam->rotate(coef * delta.y / float(window_Height), uAxis, WORLD_COORDINATES);
}


void Mouse::set_Controll_Ball(Ball * new_Ball) {
	ball = new_Ball;
}