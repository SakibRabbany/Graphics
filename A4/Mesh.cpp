#include <iostream>
#include <fstream>

#include <glm/ext.hpp>

// #include "cs488-framework/ObjFileDecoder.hpp"
#include "Mesh.hpp"

Mesh::Mesh( const std::string& fname )
	: m_vertices()
	, m_faces()
{
	std::string code;
	double vx, vy, vz;
	size_t s1, s2, s3;
    
    std::string s = "Assets/" + fname;

	std::ifstream ifs( s.c_str() );
	while( ifs >> code ) {
		if( code == "v" ) {
			ifs >> vx >> vy >> vz;
			m_vertices.push_back( glm::vec3( vx, vy, vz ) );
		} else if( code == "f" ) {
			ifs >> s1 >> s2 >> s3;
			m_faces.push_back( Triangle( s1 - 1, s2 - 1, s3 - 1 ) );
		}
	}
}

std::ostream& operator<<(std::ostream& out, const Mesh& mesh)
{
  out << "mesh {";
  /*
  
  for( size_t idx = 0; idx < mesh.m_verts.size(); ++idx ) {
  	const MeshVertex& v = mesh.m_verts[idx];
  	out << glm::to_string( v.m_position );
	if( mesh.m_have_norm ) {
  	  out << " / " << glm::to_string( v.m_normal );
	}
	if( mesh.m_have_uv ) {
  	  out << " / " << glm::to_string( v.m_uv );
	}
  }

*/
  out << "}";
  return out;
}

void Mesh::hitTest(const Ray &r, HitInformation &hit_info) {
    double t = 0;
//    std::cout << "hittesting mesh" << std::endl;
//    std::cout << "mesh size: " << m_vertices.size() << std::endl;

    for (const Triangle& face : m_faces) {
        bool res = kramer(r.origin, r.direction, m_vertices[face.v1], m_vertices[face.v2], m_vertices[face.v3], t);
//        std::cout << "t: " << t << std::endl;
        if (res and t > 0 and t < hit_info.t) {
            hit_info.t = t;
            hit_info.hit = true;
            hit_info.normal = glm::vec4(glm::normalize(glm::triangleNormal(m_vertices[face.v1], m_vertices[face.v2], m_vertices[face.v3])), 0);
            
            hit_info.hit_point = r.origin + (r.direction * t);
            std::cout << "hit the mesh" << std::endl;
        }
    }
}
