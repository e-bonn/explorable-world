#pragma once

#include "SceneNode.hpp"
#include <vector>

#include <glm/glm.hpp>

enum Axis {
	X,
	Y,
	Z
};

class DynamicObject {

	public:
	DynamicObject(SceneNode *puppet, Axis axis, glm::vec3 start, glm::vec3 max, glm::vec3 movementAmount);
	~DynamicObject();

	void move();

	SceneNode *m_object;

	Axis m_axis;
	glm::vec3 m_max;
	glm::vec3 m_movementAmount;
	glm::vec3 m_initPosn;

};