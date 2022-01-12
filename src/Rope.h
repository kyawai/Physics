#ifndef _ROPE_H_
#define _ROPE_H_

#include "DynamicObject.h"
#include <vector>

class Rope : public DynamicObject
{
public:
	Rope();
	~Rope();

	virtual void Update(float deltaTs,std::vector<Rope*> _rope_objects, bool c_pressed, Rope* rope_obj, bool z_pressed);
	void SatisfyConstraint(std::vector<Rope*> _rope_objects, float rest_distance[0]);
	void CollisionResponsesTwo(float deltaTs, std::vector<Rope*> _rope_objects, Rope* rope_obj);
	bool SphereToSphereCollision(const glm::vec3& c0, const glm::vec3& c1, float r1, float r2, glm::vec3 &cp);
private:
	glm::vec3 old_position;
	bool contMov = false;
	glm::vec3 vector;
	glm::vec3 current_distance[10];
	glm::vec3 temp;
	glm::vec3 vector1Temp; 
	glm::vec3 vector2Temp;
	glm::vec3 c0;
	float rest_distance[10];
	glm::vec3 distTwo;
	glm::vec3 distOne;
	glm::vec3 one;
	float current_distance_Value[10];
	glm::vec3 velocity_object_two;
	glm::vec3 velocity_object_one;
	int rest_count, rest_count2, rest_count3;
	float start_Pos_obj;
	bool collisionBig = false;
	bool ballPosCol = false;
};
#endif