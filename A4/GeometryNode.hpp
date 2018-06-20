#pragma once

#include "SceneNode.hpp"
#include "Primitive.hpp"
#include "Material.hpp"

class GeometryNode : public SceneNode {
public:
	GeometryNode( const std::string & name, Primitive *prim, 
		Material *mat = nullptr );

	void setMaterial( Material *material );
    
    virtual void hitTest(const Ray& r, HitInformation& hit_info);

	Material *m_material;
	Primitive *m_primitive;
};
