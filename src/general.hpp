#pragma once

#include "SceneNode.hpp"
#include <vector>

#include <glm/glm.hpp>

SceneNode* getNode(SceneNode *root, std::string name);

bool collided(SceneNode *object1, SceneNode * object2);