#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/vector_angle.hpp>

#include "Model.hpp"
#include "Camera.hpp"
#include "util.hpp"
#include "Ball.hpp"
#include "Scene_handler.hpp"

class Keyboard {
private:
	static bool keyBuffer[256];
	static Camera* cam;
	static Scene_handler* scene_Handler;
	static float coef;

public:
	static void set_Camera(Camera* camera);
	static void Set_scene_handler(Scene_handler * new_Scene_handler);

	static void special_Func(int key, int x, int y);
	static void key_board_Func(unsigned char key, int x, int y);
};

class Mouse {
private:
	static Camera* cam;
	static Ball * ball;
	static int window_Width, window_Height;
	static glm::ivec2 drag_Start_Pos;

	const static float coef;
	static bool R_BUTTON_DOWN;
	static bool L_BUTTON_DOWN;

	static int start_left_X, start_left_Y;
public:
	static void set_Camera(Camera* camera);
	static void set_Window(int width, int height);
	static void set_Controll_Ball(Ball * new_Ball);

	static void mouse_Func(int button, int state, int x, int y);
	static void mouse_Motion_Func(int x, int y);

private:
	static void apply_Change(const glm::ivec2& curPos);
	static glm::vec4 getRay(float mouse_x, float mouse_y);
};