#ifndef _K_OBJECT_
#define _K_OBJECT_

#include "GameObject.h"


class KinematicsObject : public GameObject
{
public:
	//kinematics object constructor 
	KinematicsObject();


	//destructor

	~KinematicsObject();



	virtual void Update(float deltaTs);

	void SetPosition(glm::vec3 pos) { _position = pos; }
	void SetVelocity(glm::vec3 vel) { _velocity = vel; }
	void SetScale(glm::vec3 scale) { _scale = scale; }
	glm::vec3 GetPosition() { return _position; }
	glm::mat4 GetOrientation() { return _orientation; }
	void StartSimulation(bool start) { _start = start; }


private:

	void UpdateModelMatrix();

	glm::vec3 _position;

	glm::vec3 _scale;

	bool _start = false;

	bool _hit;
	glm::vec3 _landing_pos;

	glm::vec3 _velocity;

	glm::mat4 _orientation;


	KinematicsObject* _physics_test;
};


#endif _K_OBJECT_
