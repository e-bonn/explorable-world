#pragma once

#include "SceneNode.hpp"
#include <vector>

#include <glm/glm.hpp>


struct KeyFrame {
	KeyFrame() {}
	KeyFrame(std::vector<std::string> names, std::vector<float> rots, std::vector<char> a, bool d = false) : joint_names(names), rotations(rots), axes(a), isDeletable(d) {}
	std::vector<std::string> joint_names;
	std::vector<float> rotations;
	std::vector<char> axes;
	bool isDeletable;
};

class Character {

	public:
	Character(SceneNode *puppet);
	~Character();
	
	void initKeyFrames();

	void refreshJump(float currentviewAngle);
	void jumpAnimation(float currentviewAngle);

	void refreshRun(float currentviewAngle);
	void runAnimation(float currentviewAngle);
	
	SceneNode *m_puppet;
	std::vector<KeyFrame> m_keyframes;
	
	KeyFrame *m_prevFrame;
	KeyFrame *m_frame;

	// jump trajectory calculation stuff
	bool m_midair;
	float m_theta; // in degrees
	glm::vec3 m_initPosn;
	float m_yvelocity;
	float m_xstart;
	int m_metric;
	
	static constexpr float startingHeight = 0.01;

	int m_interpolationCounter;
};

