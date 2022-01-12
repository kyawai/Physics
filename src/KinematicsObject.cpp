#include "KinematicsObject.h"
#include <GLM/gtc/type_ptr.hpp>
#include <GLM/gtc/matrix_transform.hpp>
KinematicsObject::KinematicsObject()
{
	_scale = glm::vec3(0.3f, 0.3f, 0.3f);
	_start = false;
}

void KinematicsObject::UpdateModelMatrix()
{
	_modelMatrix = glm::translate(glm::mat4(1.0f), _position);
	_modelMatrix = glm::scale(_modelMatrix, _scale);
}
void KinematicsObject::Update(float deltaTs)
{

	if(_start == true) 
	{

		glm::vec3 vf2;
	//	_velocity = GetPosition();

		_position.y += _velocity.y *deltaTs;
		vf2.y = _velocity.y + (-9.8)*deltaTs;
		_position.y += (_velocity.y + vf2.y)*0.5f*deltaTs;


		/*vf.x = _v_i.x;
		pos.x += _v_i.x *deltaTs;*/


		_position.z += _velocity.z *deltaTs;
		vf2.z = _velocity.z + (-9.8)*deltaTs;
		_position.z += (_velocity.z + vf2.z)*0.5f*deltaTs;

		_velocity = vf2;

		if (_hit == false) {
			_landing_pos = glm::vec3(_position.x, _position.y, _position.z);
		}

		if (_position.y <= 0.3)
		{
			_hit = true;
			_position = _landing_pos;
		}
		SetPosition(_position);
		
	}
	UpdateModelMatrix();
}


