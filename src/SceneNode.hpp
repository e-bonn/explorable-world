#pragma once

#include "Material.hpp"

#include <glm/glm.hpp>

#include <list>
#include <string>
#include <iostream>

struct BoundingBox
{
    BoundingBox()
    : min(glm::vec4(-0.5f,-0.5f,-0.5f,1.0f)), max(glm::vec4(0.5f,0.5f,0.5f,1.0f))
    {
    }
    void print(){
        std::cout << "MIN: " << min.x <<", " << min.y << ", " << min.z << std::endl;
        std::cout << "MAX: " << max.x <<", " << max.y << ", " << max.z << std::endl;
    }
    void reset(){
    	min = glm::vec4(-0.5f,-0.5f,-0.5f,1.0f);
	max = glm::vec4(0.5f,0.5f,0.5f,1.0f);
    }
    glm::vec4 min, max;
};

enum class NodeType {
	SceneNode,
	GeometryNode,
	JointNode
};

class SceneNode {
public:
    SceneNode(const std::string & name);

	SceneNode(const SceneNode & other);

    virtual ~SceneNode();
    
	int totalSceneNodes() const;
    
    const glm::mat4& get_transform() const;
    const glm::mat4& get_inverse() const;
    
    void set_transform(const glm::mat4& m);

    void add_child(SceneNode* child);
    
    void remove_child(SceneNode* child);

	//-- Transformations:
    void rotate(char axis, float angle, bool cascaded = false);
    void scale(const glm::vec3& amount);
    void translate(const glm::vec3& amount);

    void updateBoundingBox();

	friend std::ostream & operator << (std::ostream & os, const SceneNode & node);

    glm::vec3 distanceFromOrigin;
    glm::vec3 scales;
    glm::vec3 rotates;
    
    glm::mat4 init_trans;
    glm::vec3 init_rotates;

    // Transformations
    glm::mat4 trans;
    glm::mat4 invtrans;
    
    std::list<SceneNode*> children;

	NodeType m_nodeType;
	std::string m_name;
	unsigned int m_nodeId;

    BoundingBox m_box;

private:
	// The number of SceneNode instances.
	static unsigned int nodeInstanceCount;
};
