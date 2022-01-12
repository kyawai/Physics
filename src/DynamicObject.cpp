#include "DynamicObject.h"
#include <GLM/gtc/type_ptr.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <iostream>
DynamicObject::DynamicObject()
{
	_start = false;
	_acceleration = glm::vec3(0.0f, 2.0f, 0.0f);
	_mass = 2.0f;

	//rotation

	_force = glm::vec3(0.0f, 0.0f, 0.0f);
	_velocity = glm::vec3(0.0f, 0.0f, 0.0f);
	_mass = 2.0f;


	_torque = glm::vec3(0.0f, 0.0f, 0.0f);
	_angular_velocity = glm::vec3(0.0f, 0.0f, 0.0f);
	_angular_momentum = glm::vec3(0.0f, 0.0f, 0.0f);
	_bRadius = 0.3f;
	_R = glm::mat3(1.0f);

	_scale = glm::vec3(1.0f, 1.0f, 1.0f);
	_start = false;
	float mathTempOne = 2.0f / 5.0f;
	float mathTempTwo = _mass * _bRadius;
	float mathTempThree = mathTempOne * mathTempTwo;
	float value = pow(mathTempThree, 2);

	glm::mat3 _body_inertia_tensor = glm::mat3(value, 0, 0,
		0, value, 0,
		0, 0, value);
	_body_inertia_tensor_inverse = glm::inverse(_body_inertia_tensor);
	_inverse_inertia_tensor = _R * _body_inertia_tensor_inverse * glm::transpose(_R);
	_angular_velocity = _inverse_inertia_tensor * _angular_momentum;
}
DynamicObject::~DynamicObject()
{

}

void DynamicObject::Update(float deltaTs)
{
	if (_start == true)
	{

		//clear forces
		_force = glm::vec3(0.0f, 0.0f, 0.0f);

		//calculate forces
		_force = _mass * glm::vec3(0, -9.81, 0);

		//add forces

		CollisionResponses(deltaTs);
		_inverse_inertia_tensor = _R * _body_inertia_tensor_inverse * glm::transpose(_R);

		if (setLanded == false)
		{
			RungeKutta4(deltaTs);
		}


	}

	UpdateModelMatrix();
}
void DynamicObject::Euler(float deltaTs)
{
	_velocity += (_force / _mass) * deltaTs;
	_position += _velocity * deltaTs;

	_angular_momentum += deltaTs * _torque;
	_inverse_inertia_tensor = _R * _body_inertia_tensor_inverse * glm::transpose(_R);
	_angular_velocity = _inverse_inertia_tensor * _angular_momentum;

	glm::mat3 _skew = glm::mat3(0.0f, (-_angular_velocity.z), (_angular_velocity.y),
		(_angular_velocity.z), 0.0f, (-_angular_velocity.x),
		(-_angular_velocity.y), (_angular_velocity.x), 0.0f);
	_R += deltaTs * _angular_velocity*_R;
	std::cout << "test";


}

void DynamicObject::RungeKutta2(float deltaTs)
{
	glm::vec3 force;
	glm::vec3 acceleration;
	glm::vec3 k0;
	glm::vec3 k1;

	force = _force;
	acceleration = force / _mass;
	k0 = deltaTs * acceleration;

	force = _force + k0 / 2.0f;
	acceleration = force / _mass;
	k1 = deltaTs * acceleration;

	_velocity += k1;
	_position += _velocity * deltaTs;
}
void DynamicObject::RungeKutta4(float deltaTs)
{
	glm::vec3 force;
	glm::vec3 acceleration;
	glm::vec3 k0;
	glm::vec3 k1;
	glm::vec3 k2;
	glm::vec3 k3;

	//evaluate once at t0 to find k0

	force = _force;
	acceleration = force / _mass;
	k0 = deltaTs * acceleration;

	//evaulate twice at t0, add delta /2 using half of k0 and half of k1
	force = _force + k0 / 2.0f;
	acceleration = force / _mass;
	k1 = deltaTs * acceleration;

	force = _force + k1 / 2.0f;
	acceleration = force / _mass;
	k2 = deltaTs * acceleration;

	//Evaluate once at t0 add delta using k2

	force = _force + k2;
	acceleration = force / _mass;
	k3 = deltaTs * acceleration;

	//evaluate at t0 add delta using weighted sum of k0 k1 k2 and k3

	_velocity += (k0 + 2.0f * k1 + 2.0f * k2 + k3) / 6.0f;

	//update pos

	_position += _velocity * deltaTs;

	_angular_momentum += deltaTs * _torque;
	_inverse_inertia_tensor = _R * _body_inertia_tensor_inverse * glm::transpose(_R);
	_angular_velocity = _inverse_inertia_tensor * _angular_momentum;

	glm::mat3 _skew = glm::mat3(0.0f, (-_angular_velocity.z), (_angular_velocity.y),
		(_angular_velocity.z), 0.0f, (-_angular_velocity.x),
		(-_angular_velocity.y), (_angular_velocity.x), 0.0f);
	_R += deltaTs * _skew *_R;
}

void DynamicObject::UpdateModelMatrix()
{
	glm::mat4 R = glm::mat4(_R);
	_modelMatrix = glm::translate(glm::mat4(1.0f), _position);
	_modelMatrix = _modelMatrix * R;
	_modelMatrix = glm::scale(_modelMatrix, _scale);
}

float DynamicObject::DistanceToPlane(const glm::vec3& n, const glm::vec3& p, const glm::vec3& q)
{
	float d = glm::dot((p - q), n);
	return d;
}
bool DynamicObject::SphereToSphereCollision(const glm::vec3& c0, const glm::vec3& c1, float r1, float r2, glm::vec3 &cp)
{
	float d = glm::length(c0 - c1);
	glm::vec3 n;

	if (d <= (r1 + r2))
	{
		n = glm::normalize(c0 - c1);
		cp = r1 * n;
		return true;

	}
	return false;
}

bool DynamicObject::MovingSphereToPlaneCollision2(const glm::vec3& n, const glm::vec3& c0, const glm::vec3& c1, const glm::vec3& q, float r, glm::vec3& ci)
{
	float t;
	float d0 = DistanceToPlane(n, c0, q);
	float d1 = DistanceToPlane(n, c1, q);

	if (glm::abs(d0) <= r)
	{
		ci = c0;
		t = 0.0f;
		return true;
	}
	if (d0 >= r && d1 <= r)
	{
		t = (d0 - r) / (d0 - d1);
		ci = (1 - t)*c0 + t * c1;
		return true;
	}
	return false;
}

void DynamicObject::CollisionResponses(float deltaTs)
{

	glm::vec3 position = GetPosition();
	glm::vec3 n(0.0f, 1.0f, 0.0f);
	glm::vec3 q(0.0f, 0.0f, 0.0f);

	float d = DistanceToPlane(n, position, q);
	float bRadius = 0.3;
	glm::vec3 contactPosition;
	glm::vec3 c1 = position + _velocity * deltaTs;
	glm::vec3 c0 = position;
	bool collision = MovingSphereToPlaneCollision2(n, c0, c1, q, bRadius, contactPosition);


	if (collision)
	{
		if (firstBool == true)
		{
			friction = glm::vec3(-0.01*_mass*9.81f, 0.0f, 0.0f);
		}

		if (firstBool == false)
		{
			friction = glm::vec3(0.2*_mass*9.81f, 0.0f, 0.0f);
			firstBool = true;
		}

		glm::vec3 prevPos = GetPosition();
		std::cout << "collision ****" << std::endl;
		float impulse;
		impulse = -(1.0f + 0.8f)*glm::dot((_velocity - glm::vec3(0.0f, 0.0f, 0.0f)), n) / ((1 / _mass));
		glm::vec3 impulseF = (impulse*n) / deltaTs;
		glm::vec3 newGrav = (glm::vec3(0.0, 9.81, 0.0)*_mass);
		glm::vec3 contactForce = newGrav + impulseF;
		AddForce(contactForce);
		glm::vec3 newT = glm::cross(contactPosition - position, glm::vec3(0.0f, 0.0f, 0.0f));
		AddTorque(newT);
		if (impulse < 0.5)
		{
			setLanded = true;
		}

	}

}