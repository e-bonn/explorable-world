#include "Character.hpp"

#include "GeometryNode.hpp"
#include "JointNode.hpp"

#include <imgui/imgui.h>
#include "general.hpp"

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/io.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define INTERPOLATION_STEP_SIZE 10

using namespace std;
using namespace glm;

Character::Character(SceneNode *puppet)
	: m_puppet(puppet),
	  m_interpolationCounter(0),
	  m_prevFrame(nullptr),
	  m_frame(nullptr),
	  m_midair(false),
	  m_metric(1)
{
	initKeyFrames();
}

Character::~Character()
{
	if(m_frame && m_frame->isDeletable) delete m_frame;
	if(m_prevFrame && m_prevFrame->isDeletable) delete m_prevFrame;
}

void Character::initKeyFrames()
{
	
	// instantiate the important key frames for animation
	// will interpolate later on these important frames for 
	// smooth animation	
	
	std::vector<std::string> names;
	std::vector<float> rotations;
	std::vector<char> axes;
	names.push_back("leftHipJoint");
	rotations.push_back(0);
	axes.push_back('x');
	names.push_back("rightHipJoint");
	rotations.push_back(0);
	axes.push_back('x');
	names.push_back("leftShoulderJoint");
	rotations.push_back(0);
	axes.push_back('x');
	names.push_back("rightShoulderJoint");
	rotations.push_back(0);
	axes.push_back('x');
	m_keyframes.push_back(KeyFrame(names, rotations, axes));
	
	names.clear();
	rotations.clear();
	axes.clear();
	names.push_back("leftHipJoint");
	rotations.push_back(30);
	axes.push_back('x');
	names.push_back("rightHipJoint");
	rotations.push_back(-30);
	axes.push_back('x');
	names.push_back("leftShoulderJoint");
	rotations.push_back(-20);
	axes.push_back('x');
	names.push_back("rightShoulderJoint");
	rotations.push_back(20);
	axes.push_back('x');
	m_keyframes.push_back(KeyFrame(names, rotations, axes));
	
	// ** swing back the other waaaayyy ** //
	
	names.clear();
	rotations.clear();
	axes.clear();
	names.push_back("leftHipJoint");
	rotations.push_back(0);
	axes.push_back('x');
	names.push_back("rightHipJoint");
	rotations.push_back(0);
	axes.push_back('x');
	names.push_back("leftShoulderJoint");
	rotations.push_back(0);
	axes.push_back('x');
	names.push_back("rightShoulderJoint");
	rotations.push_back(0);
	axes.push_back('x');
	m_keyframes.push_back(KeyFrame(names, rotations, axes));
	
	names.clear();
	rotations.clear();
	axes.clear();
	names.push_back("leftHipJoint");
	rotations.push_back(-30);
	axes.push_back('x');
	names.push_back("rightHipJoint");
	rotations.push_back(30);
	axes.push_back('x');
	names.push_back("leftShoulderJoint");
	rotations.push_back(20);
	axes.push_back('x');
	names.push_back("rightShoulderJoint");
	rotations.push_back(-20);
	axes.push_back('x');
	m_keyframes.push_back(KeyFrame(names, rotations, axes));
		
	names.clear();
	rotations.clear();
	axes.clear();
	names.push_back("leftHipJoint");
	rotations.push_back(0);
	axes.push_back('x');
	names.push_back("rightHipJoint");
	rotations.push_back(0);
	axes.push_back('x');
	names.push_back("leftShoulderJoint");
	rotations.push_back(0);
	axes.push_back('x');
	names.push_back("rightShoulderJoint");
	rotations.push_back(0);
	axes.push_back('x');
	m_keyframes.push_back(KeyFrame(names, rotations, axes));

}

void Character::refreshJump(float currentviewAngle)
{
	m_midair = false;
	m_theta = 0.0f;
	m_initPosn = vec3(0,0,0);
	m_yvelocity = 0.0f;
	m_xstart = 0.0f;
	m_metric = 1;
	
	SceneNode *joint;
	
	joint = getNode(m_puppet, "leftShoulderJoint");
	
	vec3 distance = joint->distanceFromOrigin;
	joint->translate(-distance);
	m_puppet->rotate('y', -currentviewAngle);
	joint->rotate('x', -180);
	joint->translate(vec3(0,-0.21,0));
	m_puppet->rotate('y', currentviewAngle);
	joint->translate(distance);
	
	joint = getNode(m_puppet, "rightShoulderJoint");
	
	distance = joint->distanceFromOrigin;
	joint->translate(-distance);
	m_puppet->rotate('y', -currentviewAngle);
	joint->rotate('x', -180);
	joint->translate(vec3(0,-0.21,0));
	m_puppet->rotate('y', currentviewAngle);
	joint->translate(distance);
}

void Character::jumpAnimation(float currentviewAngle)
{
	SceneNode *joint;

	joint = getNode(m_puppet, "leftShoulderJoint");
	
	vec3 distance = joint->distanceFromOrigin;
	joint->translate(-distance);
	m_puppet->rotate('y', -currentviewAngle);
	joint->rotate('x', 180);
	joint->translate(vec3(0,0.21,0));
	m_puppet->rotate('y', currentviewAngle);
	joint->translate(distance);
	
	joint = getNode(m_puppet, "rightShoulderJoint");
	
	distance = joint->distanceFromOrigin;
	joint->translate(-distance);
	m_puppet->rotate('y', -currentviewAngle);
	joint->rotate('x', 180);
	joint->translate(vec3(0,0.21,0));
	m_puppet->rotate('y', currentviewAngle);
	joint->translate(distance);
}

void Character::refreshRun(float currentviewAngle)
{
	vec3 distance;
	SceneNode *joint;
	if(m_frame)
	{
		for(int i = 0; i < m_frame->joint_names.size(); i++)
		{
			joint = getNode(m_puppet, m_frame->joint_names[i]);

			if(joint)
			{
				distance = joint->distanceFromOrigin;
				joint->translate(-distance);
				m_puppet->rotate('y', -currentviewAngle);
				joint->rotate(m_frame->axes[i], -m_frame->rotations[i]);
				m_puppet->rotate('y', currentviewAngle);
				joint->translate(distance);
			}
		}
	}
	m_interpolationCounter = 0;
	m_prevFrame = nullptr;
	m_frame = nullptr;
}

void Character::runAnimation(float currentviewAngle)
{
	if(m_midair) return;

	int rem = m_interpolationCounter % INTERPOLATION_STEP_SIZE;
	bool interpolate = rem != 0;
	
	if(m_prevFrame && m_prevFrame->isDeletable) delete m_prevFrame;
	
	m_prevFrame = m_frame;
	
	int currentFrame = m_interpolationCounter / INTERPOLATION_STEP_SIZE;
	
	SceneNode *prev_joint;
	SceneNode *joint;
	
	vec3 distance;
	if (m_prevFrame)
	{
		for(int i = 0; i < m_prevFrame->joint_names.size(); i++)
		{
			prev_joint = getNode(m_puppet, m_prevFrame->joint_names[i]);
			if(prev_joint)
			{
				distance = prev_joint->distanceFromOrigin;
				prev_joint->translate(-distance);
				m_puppet->rotate('y', -currentviewAngle);
				prev_joint->rotate(m_prevFrame->axes[i], -m_prevFrame->rotations[i]);
				m_puppet->rotate('y', currentviewAngle);
				prev_joint->translate(distance);
			}
		}
	}
	
	if(m_interpolationCounter >= (m_keyframes.size()-1)*INTERPOLATION_STEP_SIZE) 
	{
		m_interpolationCounter = 0;
	}
	
	if(interpolate && m_prevFrame && m_frame)
	{	
		int nextFrame = currentFrame + 1;
		
		std::vector<std::string> names;
		std::vector<float> rotations;
		std::vector<char> axes;
		
		for(int i = 0; i < m_keyframes[currentFrame].joint_names.size(); i++)
		{
			names.push_back(m_keyframes[currentFrame].joint_names[i]);
			axes.push_back(m_keyframes[currentFrame].axes[i]);
			float step = (m_keyframes[nextFrame].rotations[i]-m_keyframes[currentFrame].rotations[i])/INTERPOLATION_STEP_SIZE;
			rotations.push_back(m_keyframes[currentFrame].rotations[i]+(step*rem));
		}

		m_frame = new KeyFrame(names,rotations,axes,true); // alloc it, make sure its deleted later

		for(int i = 0; i < m_frame->joint_names.size(); i++)
		{
			joint = getNode(m_puppet, m_frame->joint_names[i]);
	
			if(joint)
			{

				distance = joint->distanceFromOrigin;
				joint->translate(-distance);
				m_puppet->rotate('y', -currentviewAngle);
				joint->rotate(m_frame->axes[i], m_frame->rotations[i]);
				m_puppet->rotate('y', currentviewAngle);
				joint->translate(distance);
			}
		}
	}
	else if (currentFrame >= 0 && currentFrame < m_keyframes.size() && !interpolate)
	{
		m_frame = &m_keyframes[currentFrame];
		for(int i = 0; i < m_frame->joint_names.size(); i++)
		{
			joint = getNode(m_puppet, m_frame->joint_names[i]);
	
			if(joint)
			{

				distance = joint->distanceFromOrigin;
				joint->translate(-distance);
				m_puppet->rotate('y', -currentviewAngle);
				joint->rotate(m_frame->axes[i], m_frame->rotations[i]);
				m_puppet->rotate('y', currentviewAngle);
				joint->translate(distance);
			}
		}
	}
	
	if(m_interpolationCounter == 0 || m_interpolationCounter == 2*INTERPOLATION_STEP_SIZE) system("canberra-gtk-play -f step_grass.wav &");
	
	m_interpolationCounter += 1;
}

