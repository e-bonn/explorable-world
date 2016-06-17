#include "DynamicObject.hpp"

#include "GeometryNode.hpp"
#include "JointNode.hpp"

#include <imgui/imgui.h>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/io.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;
using namespace std;

DynamicObject::DynamicObject(SceneNode *object, Axis axis, vec3 start, vec3 max, vec3 movementAmount)
	: m_object(object),
	  m_axis(axis),
	  m_max(max),
	  m_initPosn(start),
	  m_movementAmount(movementAmount)
{
	if(!m_object) cout << "DynamicObject initialized without a valid object" << endl;
}

DynamicObject::~DynamicObject()
{
}

void DynamicObject::move()
{
	vec3 distanceTravelled = m_object->distanceFromOrigin - m_initPosn;

	if(distanceTravelled[m_axis] > m_max[m_axis])
	{
		m_object->translate(-m_object->distanceFromOrigin);
		m_object->translate(m_initPosn+m_max);
		m_movementAmount = vec3(-m_movementAmount.x, -m_movementAmount.y, -m_movementAmount.z);
	} 
	else if(distanceTravelled[m_axis] < 0)
	{
		m_object->translate(-m_object->distanceFromOrigin);
		m_object->translate(m_initPosn);
		m_movementAmount = vec3(-m_movementAmount.x, -m_movementAmount.y, -m_movementAmount.z);
	}
	else
	{
		m_object->translate(m_movementAmount);
	}
}