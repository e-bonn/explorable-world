#include "general.hpp"

using namespace glm;

SceneNode* getNode(SceneNode *root, std::string name)
{
	SceneNode *sn;

	if(root->m_name == name) return root;

	for (SceneNode * node : root->children) {
		sn = getNode(node, name);
		if(sn) return sn;
	}
	return nullptr;
}

bool collided(SceneNode *object1, SceneNode * object2)
{
	vec3 distanceOriginToPuppet = object1->distanceFromOrigin - object2->distanceFromOrigin;

	vec3 distancePuppet = object1->distanceFromOrigin;
	vec3 distanceObject = object2->distanceFromOrigin;

	vec3 rotate = object2->rotates;

	object2->translate(-distanceObject);
	object1->translate(-distancePuppet);
	object1->translate(distanceOriginToPuppet);

	object2->rotate('x', -rotate.x);
	object1->rotate('x', -rotate.x);
	object2->rotate('y', -rotate.y);
	object1->rotate('y', -rotate.y);
	object2->rotate('z', -rotate.z);
	object1->rotate('z', -rotate.z);

	object2->updateBoundingBox();
	object1->updateBoundingBox();

	bool min_collided = 
			(object1->m_box.min.x >= object2->m_box.min.x && object1->m_box.min.x <= object2->m_box.max.x &&
			object1->m_box.min.y >= object2->m_box.min.y && object1->m_box.min.y <= object2->m_box.max.y && 
			object1->m_box.min.z >= object2->m_box.min.z && object1->m_box.min.z <= object2->m_box.max.z);

	bool max_collided =
			(object1->m_box.max.x >= object2->m_box.min.x && object1->m_box.max.x <= object2->m_box.max.x &&
			object1->m_box.max.y >= object2->m_box.min.y && object1->m_box.max.y <= object2->m_box.max.y && 
			object1->m_box.max.z >= object2->m_box.min.z && object1->m_box.max.z <= object2->m_box.max.z);

	object2->rotate('x', rotate.x);
	object1->rotate('x', rotate.x);
	object2->rotate('y', rotate.y);
	object1->rotate('y', rotate.y);
	object2->rotate('z', rotate.z);
	object1->rotate('z', rotate.z);
;

	object1->translate(-distanceOriginToPuppet);
	object1->translate(distancePuppet);
	object2->translate(distanceObject);

	return min_collided || max_collided;
}