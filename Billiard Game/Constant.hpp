#pragma once

#include <gl/glew.h>
#include <gl/freeglut.h>

class Constant {
public:
	static const int SCREEN_WIDTH = 1000;
	static const int SCREEN_HEIGHT = 800;

	static const char * SHARE_PATH;
	static const char  * SCENE_FILE;
	static const char  * RESOURCE_FILE;

	static const float FRICTIONAL_COEFFICIENT;
	static const float TIME_FOR_A_FRAME;
};