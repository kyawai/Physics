#ifndef _DynamicObject_H_
#define _DynamicObject_H_

#include "GameObject.h"


class DynamicObject : public GameObject
{
public:

	DynamicObject();
	~DynamicObject();

	virtual void Update(float deltaTs);

	float DistanceToPlane(const glm::vec3& n, const glm::vec3& p, const glm::vec3& q);
	bool SphereToSphereCollision(const glm::vec3& c0, const glm::vec3& c1, float r1, float r2, glm::vec3& cp);
	bool MovingSphereToPlaneCollision2(const glm::vec3 & n, const glm::vec3 & c0, const glm::vec3 & c1, const glm::vec3 & q, float r, glm::vec3 & ci);

	void CollisionResponses(float deltaTs);




	void AddForce(const glm::vec3 force) { _force += force; }
	void ClearForces() { _force = glm::vec3(0.0f, 0.0f, 0.0f); }


	void Euler(float deltaTs);
	void RungeKutta2(float deltaTs);
	void RungeKutta4(float deltaTs);

	void SetForce(const glm::vec3 force) { _force = force; }
	void SetMass(float mass) { _mass = mass; }
	void SetBoundingRadius(float r) { _bRadius = r; }
	void SetPosition(const glm::vec3 pos) { _position = pos; }
	void SetVelocity(const glm::vec3 vel) { _velocity = vel; }
	void SetScale(const glm::vec3 scale) { _scale = scale; }
	const glm::vec3 GetForce()const { return _force; }
	const float GetMass() const { return _mass; }
	const float GetBoundingRadius() const { return _bRadius; }
	const glm::vec3 GetPosition()const { return _position; }
	const glm::mat4 GetOrientation() const { return _orientation; }
	void StartSimulation(bool start) { _start = start; }
	const glm::vec3 GetVelocity()const { return _velocity; }

	//rotation

	void AddTorque(const glm::vec3 torque) { _torque += torque; }
	void ClearTorques() { _torque = glm::vec3(0.0f, 0.0f, 0.0f); }



protected:
	void UpdateModelMatrix();
	glm::vec3 _force;
	glm::vec3 _position;
	glm::vec3 _currPos;
	glm::vec3 _velocity;
	glm::vec3 _currVel;
	glm::vec3 _acceleration;
	float _mass;
	float _bRadius;
	glm::vec3 _scale;
	glm::mat4 _orientation;
	bool _start;

	//rotation

	glm::vec3 _torque;
	glm::vec3 _angular_velocity;
	glm::vec3 _angular_momentum;
	glm::mat3 _inertia_tensor_inverse;
	glm::mat3 _body_inertia_tensor_inverse;
	glm::mat3 _R;
	glm::vec3 friction;
	bool firstBool = false;
	bool setLanded = false;
	glm::mat3 _inverse_inertia_tensor;

};

#endif

