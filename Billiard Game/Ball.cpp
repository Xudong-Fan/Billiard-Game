#include "Ball.hpp"


Ball::Ball()
{
	Object::Object();
	this->time_Last_Frame = 0.0;
	radius = 6.0;
	onTable = 1;
}


Ball::~Ball()
{
	Object::~Object();
}

void Ball::render(Camera * camera, Light * light, GLdouble elapsedTime)
{
	if (elapsedTime > Constant::TIME_FOR_A_FRAME){
		float coef = elapsedTime / Constant::TIME_FOR_A_FRAME;
		//cout << "Current time = " << currentTime << " lastTime = " << this->time_Last_Frame << " " << Constant::TIME_FOR_A_FRAME << endl;
		//cout << "Coef = " << coef << endl;
		this->translate(velocity.x * coef, velocity.y * coef, velocity.z * coef);

		if (glm::length(this->velocity) != 0){
			glm::vec4 rotateAxisWorld = glm::vec4(-velocity.z, 0.0f, velocity.x, 0.0f);
			glm::vec4 rotateAxisLocal = glm::inverse(this->getModelMatrix()) * rotateAxisWorld;

			float angle = glm::length(velocity) * coef / (this->get_Radius() * glm::length(this->getModelMatrix() * glm::vec4(1.0f, 0.0f, 0.0f, 0.0f)));
			//cout << "Angle = " << angle / 3.14f * 180.0f << endl;
			this->rotate(-angle / 3.1416f * 180.0f, glm::vec3(rotateAxisLocal.x, rotateAxisLocal.y, rotateAxisLocal.z));


			//cout << "Current time = " << currentTime << " lastTime = " << this->time_Last_Frame << " " << Constant::TIME_FOR_A_FRAME << endl;
			if (glm::length(this->velocity) <= glm::length(this->acceleration)){
				set_Velocity(glm::vec3(0.0f, 0.0f, 0.0f));
			}
			else {
				set_Velocity(this->velocity + this->acceleration);
			}
		}
	}
	if (onTable) {
		Object::render(camera, light, elapsedTime);
	}
	//Object::render(camera, light, elapsedTime);
}


// apply new speed to ball
void Ball::set_Velocity(glm::vec3 velocity)
{
	//cout << "set ball velocity" << velocity.x << " " << velocity.y << " " << velocity.z << endl;
	this->velocity = velocity;
	if (glm::length(this->velocity) > 0){
		this->acceleration = glm::normalize(this->velocity) * Constant::FRICTIONAL_COEFFICIENT;
		this->time_Last_Frame = glutGet(GLUT_ELAPSED_TIME);
		//cout << "Last time = " << this->time_Last_Frame << endl;
	}
	else {
		this->acceleration = glm::vec3(0.0f, 0.0f, 0.0f);
		this->time_Last_Frame = 0.0;
	}
}


// get velocity of the ball
glm::vec3 Ball::get_Velocity()
{
	return this->velocity;
}

void Ball::set_Radius(float radius){
	this->radius = radius;
}

float Ball::get_Radius(){
	return this->radius;
}

void Ball::collision_Balls(Ball * otherBall){

	if (onTable) {
		glm::vec4 other_Ball_In_World = otherBall->getModelMatrix() * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		glm::vec4 this_Ball_In_World = this->getModelMatrix() * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

		glm::vec3 collision = glm::vec3(this_Ball_In_World.x - other_Ball_In_World.x, this_Ball_In_World.y - other_Ball_In_World.y, this_Ball_In_World.z - other_Ball_In_World.z);
		//cout << "collision = " << collision << endl;
		float ball_Radius_In_World = this->get_Radius() * glm::length(this->getModelMatrix() * glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));
		//cout << "radius = " << ball_Radius_In_World << " vs " << glm::length(collision) << endl;


		//if (glm::length(collision) < 1.9995f * ball_Radius_In_World) {
		if (glm::length(collision) < 2.0f * ball_Radius_In_World) {
			mciSendString(TEXT("play Resource/Music/hit.wav"), NULL, 0, NULL);
			glm::vec3 deltaVelocity = this->get_Velocity() - otherBall->get_Velocity();
			glm::vec3 translateBack = 0.1f * glm::normalize(collision) * (glm::length(collision) - 0.0f * ball_Radius_In_World);

			this->translate(translateBack);

			glm::vec3 thisVelocity = this->get_Velocity() - ((glm::dot(deltaVelocity, collision) / (glm::length(collision) * glm::length(collision))) * collision);
			glm::vec3 otherVelocity = otherBall->get_Velocity() - ((glm::dot(-deltaVelocity, -collision) / (glm::length(-collision) * glm::length(-collision))) * -collision);
			this->set_Velocity(thisVelocity * 1.0f);
			//cout << "thisVelocity = " << thisVelocity << endl;
			otherBall->set_Velocity(otherVelocity * 1.0f);
			//cout << "otherVelocity = " << otherVelocity << endl;
		}
	}
}

void Ball::collision_Table(){

	if (onTable) {
		float radiusInWorld = this->get_Radius() * glm::length(this->getModelMatrix() * glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));
		//cout << "Radius = " << radiusInWorld << endl;

		glm::vec4 ballPositionInItsLocal = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		glm::vec4 ballInWorld4 = this->getModelMatrix() * ballPositionInItsLocal;
		glm::vec3 ballInWorld3 = glm::vec3(ballInWorld4.x, ballInWorld4.y, ballInWorld4.z);
		float absZ = (ballInWorld3.z > 0) ? ballInWorld3.z : -ballInWorld3.z;
		//cout << "AbsZ = " << absZ << endl;
		int inverseX = 1, inverseZ = 1;
		glm::vec3 oldPosition = ballInWorld3 - velocity;
		if (absZ + radiusInWorld >= 0.07) {
			inverseZ = -1;
			if (ballInWorld3.z > 0) {
				this->translate(glm::vec3(0.0f, 0.0f, -2.0f * (absZ + radiusInWorld - 0.07f)));
			}
			else {
				this->translate(glm::vec3(0.0f, 0.0f, 2.0f * (absZ + radiusInWorld - 0.07f)));
			}

		}
		float absX = (ballInWorld3.x > 0) ? ballInWorld3.x : -ballInWorld3.x;
		//cout << "absX = " << absX << endl;
		if (absX + radiusInWorld >= 0.143) {
			inverseX = -1;
			if (ballInWorld3.x > 0) {
				this->translate(glm::vec3(-2.0f * (absX + radiusInWorld - 0.143f), 0.0f, 0.0f));
			}
			else {
				this->translate(glm::vec3(2.0f * (absX + radiusInWorld - 0.143f), 0.0f, 0.0f));
			}
		}
		this->set_Velocity(glm::vec3(inverseX * velocity.x, 0.0f, inverseZ * velocity.z));
	}
}


void Ball::enter_Pocket(int objectId) {
	float radiusInWorld = this->get_Radius() * glm::length(this->getModelMatrix() * glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));
	glm::vec4 ballPositionInItsLocal = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	glm::vec4 ballInWorld4 = this->getModelMatrix() * ballPositionInItsLocal;
	glm::vec3 ballInWorld3 = glm::vec3(ballInWorld4.x, ballInWorld4.y, ballInWorld4.z);
	float absZ = (ballInWorld3.z > 0) ? ballInWorld3.z : -ballInWorld3.z;
	float absX = (ballInWorld3.x > 0) ? ballInWorld3.x : -ballInWorld3.x;

	if (absZ + radiusInWorld >= 0.068 && absX + radiusInWorld >= 0.141) {
		mciSendString(TEXT("play Resource/Music/ball_in.wav"), NULL, 0, NULL);
		//cout << "absX = " << ballInWorld3.x << endl;
		if (objectId == 2) {
			this->translate(glm::vec3(-ballInWorld4.x + -0.075f, 0.0f, -ballInWorld4.z + 0.0f));
			this->set_Velocity(glm::vec3(0.0f, 0.0f, 0.0f));

		}
		else {
			this->translate(glm::vec3(-ballInWorld4.x + -0.075f + (objectId - 3) * 2 * radiusInWorld, 0.012f, -ballInWorld4.z + 0.075f));
			this->set_Velocity(glm::vec3(0.0f, 0.0f, 0.0f));
			onTable = 0;
		}
	}
	else this->collision_Table();

}
