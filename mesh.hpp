#ifndef _MESH_HPP_
#define _MESH_HPP_

#include <map>
#include <utility>
#include <vector>

#include "SDL.h"
#include "SDL_opengl.h"

#include "vector.hpp"

namespace NEWTON {

struct Triangle {
    unsigned int vertices[3];
};

typedef std::pair<size_t, size_t> edge;

class Mesh {
public:
	Mesh() : vertices(0), triangles(0), normals(0)  {}

	void construct_sphere(real_t r);

	void subdivide();
	void make_spherical(real_t r);
	void render();

private:

	void update_arrays();

	std::vector<Vector3> vertices_vec;
	std::vector<Triangle> triangles_vec;

	Vector3 * vertices;
	size_t num_vertices;
	Triangle * triangles;
	size_t num_triangles;
	Vector3 * normals;

};

} // NEWTON

#endif