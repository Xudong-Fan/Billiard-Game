#pragma once
#include <windows.h>

#include "Object.hpp"
#include  "Constant.hpp"
class Ball :
	public Object
{
public:
	Ball();
	~Ball();

	virtual void render(Camera * camera, Light * light, GLdouble elapsedTime);
	// Apply new velocity to ball
	void set_Velocity(glm::vec3 velocity);
	// Get velocity of the ball
	glm::vec3 get_Velocity();

	void set_Radius(float radius);
	float get_Radius();

	void collision_Balls(Ball * otherBall);
	void collision_Table();
	void enter_Pocket(int objectID);
private:
	glm::vec3 velocity;
	glm::vec3 acceleration;
	int onTable;

	float radius;

	GLdouble time_Last_Frame;
};