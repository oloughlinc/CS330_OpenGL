#ifndef SHAPES_H
#define SHAPES_H

#include "openGLmesh.h"

/*
* A collection of OpenGL Mesh shapes. These are a grouping of vertices and indices. The vertex array
* also carries color information which this projects OpenGL controller is set to recognize based on params
* for floatsPerVertex and floatsPerColor and handle accordingly. 
* 
* All shapes here will be rendered as a collection of triangles as defined in the indices.
* 
*/

namespace shapes {

	OpenGLMesh Pyramid() {

		OpenGLMesh pyramid;

		pyramid.vertices = {

			// Vertex Positions (x, y, z)    // Colors (r,g,b,a)
			// base 
			0.0f, 0.0f, 0.0f,			0.0f, 1.0f, 1.0f, 1.0f, //cyan
			0.0f, 0.0f, -1.0f,			1.0f, 0.0f, 0.0f, 1.0f, // red 
			1.0f, 0.0f, -1.0f,			1.0f, 1.0f, 0.0f, 1.0f, //yellow
			1.0f, 0.0f, 0.0f,			1.0f, 0.0f, 1.0f, 1.0f, //magenta

		   // tip
			0.5f, 1.0f, -0.5f,			1.0f, 1.0f, 1.0f, 1.0f, // ALL (white)
		};

		pyramid.indices = {

			0, 1, 3, // base 1
			1, 2, 3, // base 2
			0, 3, 4, // side
			3, 2, 4, // side
			2, 1, 4, // side
			1, 0, 4, // side

		};

		pyramid.floatsPerVertex = 3;
		pyramid.floatsPerColor = 4;

		return pyramid;
	}

	OpenGLMesh Cube() {

		OpenGLMesh cube;

		cube.vertices = {

			//bottom 4 vertices
			0.0f, 0.0f, 0.0f,			0.0f, 1.0f, 1.0f, 1.0f, //cyan
			0.0f, 0.0f, -1.0f,			1.0f, 0.0f, 0.0f, 1.0f, // red 
			1.0f, 0.0f, -1.0f,			1.0f, 1.0f, 0.0f, 1.0f, //yellow
			1.0f, 0.0f, 0.0f,			1.0f, 0.0f, 1.0f, 1.0f, //magenta

			//top four vertices
			0.0f, 1.0f, 0.0f,			0.0f, 1.0f, 1.0f, 1.0f, //cyan
			0.0f, 1.0f, -1.0f,			1.0f, 0.0f, 0.0f, 1.0f, // red 
			1.0f, 1.0f, -1.0f,			1.0f, 1.0f, 0.0f, 1.0f, //yellow
			1.0f, 1.0f, 0.0f,			1.0f, 0.0f, 1.0f, 1.0f, //magenta
		};

		cube.indices = {

			0, 1, 3, // base 1
			1, 2, 3, // base 2
			0, 4, 3, // front 1
			3, 7, 4, // front 2
			3, 7, 2, // r side 1
			2, 7, 6, // r side 2
			0, 1, 4, // l side 1
			1, 4, 5, // l side 2
			1, 5, 2, // back 1
			2, 5, 6, // back 2
			4, 5, 7, // top 1
			5, 6, 7, // top 2
		};

		cube.floatsPerVertex = 3;
		cube.floatsPerColor = 4;

		return cube;
	}

	OpenGLMesh Plane() {

		OpenGLMesh plane;

		plane.vertices = {

			0.0f, 0.0f, 0.0f,			1.0f, 1.0f, 1.0f, 1.0f, //white
			0.0f, 0.0f, -1.0f,			1.0f, 1.0f, 1.0f, 1.0f, // white 
			1.0f, 0.0f, -1.0f,			1.0f, 1.0f, 1.0f, 1.0f, // white
			1.0f, 0.0f, 0.0f,			1.0f, 1.0f, 1.0f, 1.0f, //white
		};

		plane.indices = {

			0, 1, 3, // base 1
			1, 2, 3, // base 2
		};

		plane.floatsPerVertex = 3;
		plane.floatsPerColor = 4;

		return plane;
	}

}

#endif
