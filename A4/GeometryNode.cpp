#include "GeometryNode.hpp"

//---------------------------------------------------------------------------------------
GeometryNode::GeometryNode(
	const std::string & name, Primitive *prim, Material *mat )
	: SceneNode( name )
	, m_material( mat )
	, m_primitive( prim )
    , m_texture( nullptr )
{
	m_nodeType = NodeType::GeometryNode;
}

void GeometryNode::setMaterial( Material *mat )
{
	// Obviously, there's a potential memory leak here.  A good solution
	// would be to use some kind of reference counting, as in the 
	// C++ shared_ptr.  But I'm going to punt on that problem here.
	// Why?  Two reasons:
	// (a) In practice we expect the scene to be constructed exactly
	//     once.  There's no reason to believe that materials will be
	//     repeatedly overwritten in a GeometryNode.
	// (b) A ray tracer is a program in which you compute once, and 
	//     throw away all your data.  A memory leak won't build up and
	//     crash the program.

	m_material = mat;
}

void GeometryNode::hitTest(const Ray &r, HitInformation &hit_info) {
    glm::vec4 trans_origin, trans_direction;
    trans_origin = invtrans * r.origin;
    trans_direction = invtrans * r.direction;
    Ray transformed_ray(trans_origin, trans_direction);
    
    HitInformation prim_hit_info = HitInformation(hit_info.incident_ray);
    
    m_primitive->hitTest(transformed_ray, prim_hit_info);
    
//    std::cout << "in geometry node: " << m_name << std::endl;
    
    if (prim_hit_info.hit){
        if (!hit_info.hit) {
            hit_info.hit = true;
            hit_info.t = prim_hit_info.t;
//            glm::mat3 inv = glm::mat3(inverse_world_mat);
//            hit_info.normal = glm::vec4((glm::transpose(inv) * glm::vec3(prim_hit_info.normal)),0);
            hit_info.normal = prim_hit_info.normal;
            hit_info.node = this;
            hit_info.phong_mat = (PhongMaterial*) m_material;
            hit_info.hit_point = prim_hit_info.hit_point;
            hit_info.u = prim_hit_info.u;
            hit_info.v = prim_hit_info.v;
        } else {
            if (prim_hit_info.t < hit_info.t) {
                hit_info.t = prim_hit_info.t;
//                glm::mat3 inv = glm::mat3(inverse_world_mat);
//                hit_info.normal = glm::vec4((glm::transpose(inv) * glm::vec3(prim_hit_info.normal)),0);
                hit_info.normal = prim_hit_info.normal;
                hit_info.node = this;
                hit_info.phong_mat = (PhongMaterial*) m_material;
                hit_info.hit_point = prim_hit_info.hit_point;
                hit_info.u = prim_hit_info.u;
                hit_info.v = prim_hit_info.v;
            }
        }
    }

    
    for (SceneNode* child : children) {
        child->hitTest(transformed_ray, hit_info);
    }

    if (hit_info.hit) {
        hit_info.hit_point = trans * hit_info.hit_point;
        glm::mat3 inverse = glm::mat3(invtrans);
        hit_info.normal = glm::normalize(glm::vec4((glm::transpose(inverse) * glm::vec3(hit_info.normal)),0));
    }
}
