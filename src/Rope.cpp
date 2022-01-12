#include "Rope.h"
#include <GLM/gtc/type_ptr.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <iostream>
#include <math.h>

Rope::Rope()
{
	// rope constructor
	_scale = glm::vec3(0.3f, 0.3f, 0.3f);
	_start = false;
	_bRadius = 0.3f;
	_R = glm::mat3(1.0f);
	_acceleration = glm::vec3(0.0f, 2.0f, 0.0f);

	_force = glm::vec3(0.0f, 0.0f, 0.0f);
	_velocity = glm::vec3(0.0f, 0.0f, 0.0f);
	_mass = 15.0f;
	rest_count = 1;
	rest_count2 = 1;
	rest_count3 = 1;
}
Rope::~Rope()
{}
void Rope::Update(float deltaTs, std::vector<Rope*> _rope_objects, bool c_pressed, Rope* rope_obj, bool z_pressed)
{
	rope_obj->SetScale(glm::vec3(0.1f, 0.1f, 0.1f));
	if (rest_count2 == 1)
	{
		start_Pos_obj = -5.0f;
		rest_count2++;
	}
	if (rest_count3 == 1) // move onto another bool as I am unable to change the _pressed bool in this function
	{
		if (c_pressed == true)
		{
			std::cout << "c pressed " << std::endl;
			ballPosCol = true;
			rest_count3++;
		}
	}
	if (_start == true)
	{
			//clear forces
			_force = glm::vec3(0.0f, 0.0f, 0.0f);

			//calculate forces
			_force = _mass * glm::vec3(0, -9.81, 0);

			//add forces
			_acceleration = glm::vec3(0.0f, 0.0f, 0.0f);
			//contraint
			if (ballPosCol == true)  //checks if button c was entered, if entered reset the balls pos and move main ball to the side
			{
				collisionBig = false;
				rest_count = 1;
				ballPosCol = false;
				for (int i = 1; i < _rope_objects.size(); i++)
				{
					_rope_objects[i]->SetPosition(glm::vec3(0.0f, (20.0f - i) - (i / 5.0f), 0.0f));
					_rope_objects[0]->SetPosition(glm::vec3(0.0f, 20.0f, 0.0f)); //set pos to where it was before to stop the other balls from exploding straight away
					_rope_objects[0]->SetVelocity(glm::vec3(0.0f, 0.1f, 0.0f));
				}
			
				_rope_objects[0]->SetPosition(glm::vec3(0.5f, 20.0f, 0.0f));
			}
			else
			{
				_rope_objects[0]->SetPosition(glm::vec3(0.0f, 20.0f, 0.0f)); //otherwise keep main ball at same pos
			}

			if (collisionBig == false)  //check if the balls have colided with sword
			{

				SatisfyConstraint(_rope_objects, rest_distance);
			}

			if (z_pressed == true)  //check if key was pressed : start moving sword
			{
				if (rope_obj->GetPosition() == glm::vec3(5.0f, 17.0f, 0.0f)) { z_pressed = false; }
				rope_obj->SetPosition(glm::vec3((start_Pos_obj+=0.05f), 17.0f, 0.0f));

			}
			else
			{
				rope_obj->SetPosition(glm::vec3(-5.0f, 17.0f, 0.0f));  //otherwise keep the sword at same place
			}

			CollisionResponses(deltaTs);
			CollisionResponsesTwo(deltaTs, _rope_objects,rope_obj);
			if (setLanded == false)
			{
				RungeKutta4(deltaTs); 
			}

		}
		_rope_objects[0]->SetVelocity(glm::vec3(0.0f, 0.1f, 0.0f));
		UpdateModelMatrix();
}


void Rope::SatisfyConstraint(std::vector<Rope*> _rope_objects, float rest_distance[])
{
	if (rest_count == 1)
	{
		//set rest distance only once
		for (int i = 0; i < (_rope_objects.size() - 1); i++)
		{
			vector1Temp = _rope_objects[i]->GetPosition();
			vector2Temp = _rope_objects[i + 1]->GetPosition();
			vector = vector1Temp - vector2Temp;
			rest_distance[i] = glm::length(vector);
		}
		rest_count++;
	}
	//get the positions of the rope objects
	for (int i = 0; i < (_rope_objects.size() - 1); i++)
	{
		distOne = _rope_objects[i]->GetPosition();
		distTwo = _rope_objects[i + 1]->GetPosition();
		temp = distOne - distTwo;
		current_distance[i] = temp;
		current_distance_Value[i] = glm::length(temp);

	}
	for (int i = 0; i < (_rope_objects.size() - 1); i++)
	{
		if (i < 2)
		{

			//spring
			float distance_Extension = current_distance_Value[i] - rest_distance[i];  //extension
			float spring_stretch = (distance_Extension * 400.0f);  //spring/stretch
			glm::vec3 spring_difference = current_distance[i] / current_distance_Value[i];
			velocity_object_two = _rope_objects[i]->GetVelocity();
			velocity_object_one = _rope_objects[i + 1]->GetVelocity();
			glm::vec3 velocity_difference = (velocity_object_two - velocity_object_one); //get the velocity difference
			float damp = 390.0f * glm::dot(velocity_difference, spring_difference); //calculate the damp coefficient
			glm::vec3 spring = spring_difference * (spring_stretch + damp); //calculate the whole spring force
			AddForce(spring); // add the force
			_rope_objects[i]->AddForce(-spring);
			if (i - 1 > 0)
			{
				_rope_objects[i - 1]->AddForce(spring);
			}
			if (i + 1 < _rope_objects.size())
			{
				_rope_objects[i + 1]->AddForce(spring);
			}
		}
		else
		{
			//spring
			float distance_Extension = current_distance_Value[i] - rest_distance[i];  //extension
			float spring_stretch = (distance_Extension * 200.0f);  //spring/stretch
			glm::vec3 spring_difference = current_distance[i] / current_distance_Value[i];
			velocity_object_two = _rope_objects[i]->GetVelocity();
			velocity_object_one = _rope_objects[i + 1]->GetVelocity();
			glm::vec3 velocity_difference = (velocity_object_two - velocity_object_one); //get the velocity difference
			float damp = 190.0f * glm::dot(velocity_difference, spring_difference); //calculate the damp coefficient
			glm::vec3 spring = spring_difference * (spring_stretch + damp); //calculate the whole spring force
			AddForce(spring); // add the force
			_rope_objects[i]->AddForce(-spring);
			if (i - 1 > 0)
			{
				_rope_objects[i - 1]->AddForce(spring);
			}
			if (i + 1 < _rope_objects.size())
			{
				_rope_objects[i + 1]->AddForce(spring);
			}

			//below: bending resistance code that I decided to take out


			/*if (i - 2 > 0)
			{
				_rope_objects[i - 2]->AddForce(spring);
			}
			if (i + 2 < _rope_objects.size())
			{
				_rope_objects[i + 2]->AddForce(spring);
			}*/
		}

	}	
	rest_count++;	
	contMov = false;
	collisionBig = false;

}
void Rope::CollisionResponsesTwo(float deltaTs, std::vector<Rope*> _rope_objects, Rope* rope_obj) //collision code between balls
{
	for (int i = 0; i < (_rope_objects.size()-1); i++) //collisions between the rope balls
	{
		glm::vec3 position = GetPosition();
		glm::vec3 n(0.0f, 1.0f, 0.0f);
		glm::vec3 q(0.0f, 0.0f, 0.0f);

		float bRadius = 0.12f;
		glm::vec3 contactPosition;
		glm::vec3 c1 = _rope_objects[i]->GetPosition();
		c0 = _rope_objects[i + 1]->GetPosition();
		bool collision = SphereToSphereCollision(c0, c1, bRadius, bRadius, contactPosition);
		if (collision)
		{
			contMov = true;
		//	std::cout << "collision between balls" << std::endl;
		}
		if (!collision)
		{
			contMov = false;
		}
	}
	for (int i = 0; i < _rope_objects.size(); i++) //collision between balls and sword
	{
		glm::vec3 position = GetPosition();
		glm::vec3 n(0.0f, 1.0f, 0.0f);
		glm::vec3 q(0.0f, 0.0f, 0.0f);

		float bRadius = 0.12f;
		float bigRadius = 1.0f;
		glm::vec3 contactPosition;
		glm::vec3 c1 = _rope_objects[i]->GetPosition();
		c0 = rope_obj->GetPosition();
		bool collision = SphereToSphereCollision(c0, c1, bRadius, bigRadius, contactPosition);
		if (collision)
		{
			collisionBig = true;
			std::cout << "collision between balls and sword" << std::endl;
		}
	}
	

}
bool Rope::SphereToSphereCollision(const glm::vec3& c0, const glm::vec3& c1, float r1, float r2, glm::vec3 &cp) //collision detection
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

