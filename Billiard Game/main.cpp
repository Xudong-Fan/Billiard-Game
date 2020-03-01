#ifdef _WIN32
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "glut32.lib")
#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "devil.lib")
#pragma comment(lib, "ilu.lib")
#pragma comment(lib, "ilut.lib")
#pragma comment(lib, "assimp.lib")
#pragma comment(lib,"winmm.lib")
#endif

#include <gl/glew.h>
#include <gl/freeglut.h>
#include <glm/glm.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <thread>
#include <windows.h>
#include <mmsystem.h>


#include "Camera.hpp"
#include "vertex.hpp"
#include "Scene_handler.hpp"
#include "Resource_handler.hpp"
#include "Constant.hpp"
#include "Ball.hpp"
#include "Play_conf.hpp"


using namespace std;

Scene_handler scene_handler;
Resource_handler resource_handler;

void init(int argc, char *argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA | GLUT_MULTISAMPLE);
	//glutInitWindowSize(Constant::SCREEN_WIDTH, Constant::SCREEN_HEIGHT); // Initialize window size
	glutInitWindowPosition(460, 100); // Distance to the left of the screen: 460. Distance to the top of the screen: 100
	glutInitWindowSize(1000, 800); // Initialize window size
    
	// "glutCreateWindow" creates a top-level window. The name will be provided to the window system as the window's name.
	glutCreateWindow("Virtual Reality Project---Xudong Fan & Luke Burke");

	GLenum res = glewInit();
	if (res != GLEW_OK) {
		fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
	}
	fprintf(stdout, "======================= System Info =======================\n");
	fprintf(stdout, "\tVendor: %s\n", glGetString(GL_VENDOR));
	fprintf(stdout, "\tRenderer: %s\n", glGetString(GL_RENDERER));
	fprintf(stdout, "\tGL Version: %s\n", glGetString(GL_VERSION));
	fprintf(stdout, "\tGLSL Version: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
	fprintf(stdout, "\tGLEW Version: %s\n", glewGetString(GLEW_VERSION));
	fprintf(stdout, "===========================================================\n");

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glEnable(GL_POINT_SMOOTH);
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_POLYGON_OFFSET_LINE);
	glEnable(GL_DEPTH_TEST);

	resource_handler.load(Constant::RESOURCE_FILE);
	scene_handler.load(Constant::SCENE_FILE, &resource_handler);
}

void displayFunc() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	scene_handler.renderScene(0);
	//cout << "View point = " << scene_handler.getScene(0)->get_Using_Camera()->getViewPoint() << endl;
	glutSwapBuffers();
}

void idleFunc() {
	glutPostRedisplay();
}

void background_music()
{
	PlaySound(TEXT("Resource/Music/background.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
}

int main(int argc, char *argv[]) {
	std::thread t(background_music);
	t.detach();

	init(argc, argv);

	// Now you have to tell GLUT to use which function above to render. This is called the registration callback.
	// When you need to redraw, GLUT has a function that only passes a function name parameter (a function with a function name parameter) will be called.
	glutDisplayFunc(displayFunc);

	// Then we have to tell GLUT that the render function should be called when the application is idle. 
	// This causes the GLUT to always call the render function to animate.
	glutIdleFunc(idleFunc);
	scene_handler.getScene(0)->set_Camera(0);
	Keyboard::set_Camera(scene_handler.getScene(0)->get_Using_Camera());
	Keyboard::Set_scene_handler(&scene_handler);
	glutSpecialFunc(Keyboard::special_Func);
	glutKeyboardFunc(Keyboard::key_board_Func); // Operation with the keyboard

	Mouse::set_Camera(scene_handler.getScene(0)->get_Using_Camera());
	//Mouse::set_Window(Constant::SCREEN_WIDTH, Constant::SCREEN_HEIGHT);
	Mouse::set_Controll_Ball((Ball *)(scene_handler.getScene(0)->objects[2]));
	glutMouseFunc(Mouse::mouse_Func); //Operation with the mouse
	glutMotionFunc(Mouse::mouse_Motion_Func);


	// The last thing is to tell GLUT that we are ready to enter the application event processing loop. 
	// GLUT provides a function to get the program into a loop that never ends. Always waiting to process the next event.
	glutMainLoop();

	scene_handler.unload();
	resource_handler.unload();

	return 0;
}



