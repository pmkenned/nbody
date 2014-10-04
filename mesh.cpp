#include "mesh.hpp"

namespace NEWTON {

// NOTE: this subroutine creates new points on the midpoints
// of edges, not according to the "loop subdivision" algorithm
void Mesh::subdivide() {

	std::map<edge, size_t > new_vertices; // TODO: can this be reused between subdivisions?

	size_t n = triangles_vec.size();
	size_t new_index = vertices_vec.size();
	for(size_t i=0; i < n; i++) {
		Triangle & t = triangles_vec[i];
		size_t v0 = t.vertices[0];
		size_t v1 = t.vertices[1];
		size_t v2 = t.vertices[2];

		size_t sum = v0 + v1 + v2;
		size_t a = (v0 < v1 && v0 < v2) ? v0 : (v1 < v0 && v1 < v2) ? v1 : v2;
		size_t c = (v0 > v1 && v0 > v2) ? v0 : (v1 > v0 && v1 > v2) ? v1 : v2;
		size_t b = sum - a - c;

		edge ab(a,b);
		edge bc(b,c);
		edge ac(a,c);

		size_t d, e, f;

		if ( new_vertices.find(ab) == new_vertices.end() ) {
			Vector3 a_v = vertices_vec[a];
			Vector3 b_v = vertices_vec[b];
			vertices_vec.push_back((a_v + b_v)/2);
			d = new_index++;
			new_vertices[ab] = d;
		}
		else {
			d = new_vertices[ab];
		}

		if ( new_vertices.find(bc) == new_vertices.end() ) {
			Vector3 b_v = vertices_vec[b];
			Vector3 c_v = vertices_vec[c];
			vertices_vec.push_back((b_v + c_v)/2);
			e = new_index++;
			new_vertices[bc] = e;
		}
		else {
			e = new_vertices[bc];
		}

		if ( new_vertices.find(ac) == new_vertices.end() ) {
			Vector3 a_v = vertices_vec[a];
			Vector3 c_v = vertices_vec[c];
			vertices_vec.push_back((a_v + c_v)/2);
			f = new_index++;
			new_vertices[ac] = f;
		}
		else {
			f = new_vertices[ac];
		}

		// add new triangles
		t.vertices[0] = d;
		t.vertices[1] = e;
		t.vertices[2] = f;
		Triangle t0 = {a,d,f};
		Triangle t1 = {b,d,e};
		Triangle t2 = {e,f,c};
		triangles_vec.push_back(t0);
		triangles_vec.push_back(t1);
		triangles_vec.push_back(t2);
	}
	update_arrays();
}

void Mesh::update_arrays() {
	num_vertices = vertices_vec.size();
	num_triangles = triangles_vec.size();

	if(vertices != 0) delete[] vertices;
	if(triangles != 0) delete[] triangles;
	if(normals != 0) delete[] normals;

	vertices = new Vector3[num_vertices];
	triangles = new Triangle[num_triangles];
	normals = new Vector3[num_vertices];

	std::vector<Vector3>::iterator v;
	size_t i = 0;
	for(v = vertices_vec.begin(); v < vertices_vec.end(); v++) {
		vertices[i] = *v;
		i++;
	}
	std::vector<Triangle>::iterator t;
	i=0;
	for(t = triangles_vec.begin(); t < triangles_vec.end(); t++) {
		triangles[i] = *t;
		i++;
	}
}

void Mesh::construct_sphere(real_t r) {

	// equilateral triangle
	vertices_vec.push_back(r * Vector3(1,1,0));
	vertices_vec.push_back(r * Vector3(-1,1,0));
	vertices_vec.push_back(r * Vector3(-1,-1,0));
	vertices_vec.push_back(r * Vector3(1,-1,0));
	vertices_vec.push_back(r * Vector3(0,0,1.414));
	vertices_vec.push_back(r * Vector3(0,0,-1.414));
	
	// top pyramid
	Triangle tri0 = {0,3,4};
	Triangle tri1 = {0,1,4};
	Triangle tri2 = {1,2,4};
	Triangle tri3 = {2,3,4};

	// bottom pyramid
	Triangle tri4 = {0,3,5};
	Triangle tri5 = {0,1,5};
	Triangle tri6 = {1,2,5};
	Triangle tri7 = {2,3,5};

	triangles_vec.push_back(tri0);
	triangles_vec.push_back(tri1);
	triangles_vec.push_back(tri2);
	triangles_vec.push_back(tri3);
	triangles_vec.push_back(tri4);
	triangles_vec.push_back(tri5);
	triangles_vec.push_back(tri6);
	triangles_vec.push_back(tri7);

	update_arrays();
}

void Mesh::render() {
	
	for(size_t i = 0; i < num_triangles; i++) {
		glBegin(GL_LINE_LOOP);
		for(size_t v = 0; v < 3; v++) {
			Vector3 vx = vertices[triangles[i].vertices[v]];
			glVertex3d(vx.x, vx.y, vx.z);
		}
		glEnd();
	}
}

void Mesh::make_spherical(real_t r) {

	std::vector<Vector3>::iterator v;
	for(v = vertices_vec.begin(); v != vertices_vec.end(); v++) {
		*v = r * normalize(*v);
	}

	update_arrays();
}

} // NEWTON