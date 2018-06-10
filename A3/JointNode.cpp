#include "JointNode.hpp"
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include "cs488-framework/MathUtils.hpp"

using namespace glm;
//---------------------------------------------------------------------------------------
JointNode::JointNode(const std::string& name)
	: SceneNode(name)
{
	m_nodeType = NodeType::JointNode;
}

//---------------------------------------------------------------------------------------
JointNode::~JointNode() {

}
 //---------------------------------------------------------------------------------------
void JointNode::set_joint_x(double min, double init, double max) {
	m_joint_x.min = min;
	m_joint_x.init = init;
	m_joint_x.max = max;
    
    this->rotate('x', m_joint_x.init);
    angle_x = init;
}

//---------------------------------------------------------------------------------------
void JointNode::set_joint_y(double min, double init, double max) {
	m_joint_y.min = min;
	m_joint_y.init = init;
	m_joint_y.max = max;
    
    this->rotate('y', m_joint_y.init);
    angle_y = init;
}

void JointNode::rotate(char axis, float angle) {
    
    vec3 rot_axis;
    
    switch (axis) {
        case 'x':
            if (angle_x + angle > m_joint_x.max) {
                angle = m_joint_x.max - angle_x;
                angle_x = m_joint_x.max;
            } else if (angle_x + angle < m_joint_x.min) {
                angle = m_joint_x.min - angle_x;
                angle_x = m_joint_x.min;
            } else {
                angle_x += angle;
            }
            rot_axis = vec3(1,0,0);
            break;
        case 'y':
            if (angle_y + angle > m_joint_y.max) {
                angle = m_joint_y.max - angle_y;
                angle_y = m_joint_y.max;
            } else if (angle_y + angle < m_joint_y.min) {
                angle = m_joint_y.min - angle_y;
                angle_y = m_joint_y.min;
            } else {
                angle_y += angle;
            }
            rot_axis = vec3(0,1,0);
            break;
        case 'z':
            rot_axis = vec3(0,0,1);
            break;
        default:
            break;
    }
    mat4 rot_matrix = glm::rotate(degreesToRadians(angle), rot_axis);
    trans = rot_matrix * trans;
}
