#ifndef SHAPES_H
#define SHAPES_H

#include "openGLmesh.h"
#include <algorithm> //reverse vector

/*
* A collection of OpenGL Mesh shapes. These are a grouping of vertices and indices. The vertex array
* also carries color information which this projects OpenGL controller is set to recognize based on params
* for floatsPerVertex and floatsPerColor and handle accordingly. 
* 
* All shapes here will be rendered as a collection of triangles as defined in the indices.
* 
*/

namespace shapes {

#define GREY std::vector<float>() = {0.5f,0.5f,0.5f,1.0f}
#define UP_NORM std::vector<float>() = {0.0f,1.0f,0.0f}
#define DOWN_NORM std::vector<float>() = {0.0f,-1.0f,0.0f}

	template <typename T>
	void append(std::vector<T>& vertices, std::vector<T>color) {
		vertices.insert(vertices.end(), color.begin(), color.end());
	}

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

			//bottom					// color						//uv mapping		//normals
			0.0f, 0.0f, 0.0f,			0.0f, 1.0f, 1.0f, 1.0f,			0.0f, 0.2f,			0.0f, -1.0f, 0.0f,
			0.0f, 0.0f, -1.0f,			1.0f, 0.0f, 0.0f, 1.0f,			0.65f, 0.2f,		0.0f, -1.0f, 0.0f,
			1.0f, 0.0f, -1.0f,			1.0f, 1.0f, 0.0f, 1.0f,			0.65f, 1.0f,		0.0f, -1.0f, 0.0f,
			1.0f, 0.0f, 0.0f,			1.0f, 0.0f, 1.0f, 1.0f,			0.0f, 1.0f,			0.0f, -1.0f, 0.0f,

			//top
			0.0f, 1.0f, 0.0f,			0.0f, 1.0f, 1.0f, 1.0f,			0.65f, 0.2f,		0.0f, 1.0f, 0.0f,
			0.0f, 1.0f, -1.0f,			1.0f, 0.0f, 0.0f, 1.0f,			0.0f, 0.2f,			0.0f, 1.0f, 0.0f,
			1.0f, 1.0f, -1.0f,			1.0f, 1.0f, 0.0f, 1.0f,			0.0f, 1.0f,			0.0f, 1.0f, 0.0f,
			1.0f, 1.0f, 0.0f,			1.0f, 0.0f, 1.0f, 1.0f,			0.65f, 1.0f,		0.0f, 1.0f, 0.0f,

			//front
			0.0f, 0.0f, 0.0f,			0.0f, 1.0f, 1.0f, 1.0f,			0.0f, 0.0f,			0.0f, 0.0f, -1.0f,
			1.0f, 0.0f, 0.0f,			1.0f, 0.0f, 1.0f, 1.0f,			0.98f, 0.0f,		0.0f, 0.0f, -1.0f,
			1.0f, 1.0f, 0.0f,			0.0f, 1.0f, 1.0f, 1.0f,			0.98f, 0.13f,		0.0f, 0.0f, -1.0f,
			0.0f, 1.0f, 0.0f,			1.0f, 0.0f, 1.0f, 1.0f,			0.0f, 0.13f,		0.0f, 0.0f, -1.0f,

			//back
			0.0f, 0.0f, -1.0f,			1.0f, 0.0f, 0.0f, 1.0f,			0.98f, 0.0f,		0.0f, 0.0f, 1.0f,
			1.0f, 0.0f, -1.0f,			1.0f, 1.0f, 0.0f, 1.0f,			0.0f, 0.0f,			0.0f, 0.0f, 1.0f,
			1.0f, 1.0f, -1.0f,			1.0f, 0.0f, 0.0f, 1.0f,			0.0f, 0.13f,		0.0f, 0.0f, 1.0f,
			0.0f, 1.0f, -1.0f,			1.0f, 1.0f, 0.0f, 1.0f,			0.98f, 0.13f,		0.0f, 0.0f, 1.0f,

			//left 
			0.0f, 0.0f, 0.0f,			0.0f, 1.0f, 1.0f, 1.0f,			1.0f, 0.77f,		-1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, -1.0f,			1.0f, 0.0f, 0.0f, 1.0f,			1.0f, 0.135f,		-1.0f, 0.0f, 0.0f,
			0.0f, 1.0f, -1.0f,			0.0f, 1.0f, 1.0f, 1.0f,			0.83f, 0.135f,		-1.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,			1.0f, 0.0f, 0.0f, 1.0f,			0.83f, 0.77f,		-1.0f, 0.0f, 0.0f,

			//right	
			1.0f, 0.0f, 0.0f,			1.0f, 1.0f, 0.0f, 1.0f,			0.8f, 0.14f,		1.0f, 0.0f, 0.0f,
			1.0f, 0.0f, -1.0f,			1.0f, 0.0f, 1.0f, 1.0f,			0.8f, 0.77f,		1.0f, 0.0f, 0.0f,
			1.0f, 1.0f, -1.0f,			1.0f, 1.0f, 0.0f, 1.0f,			0.675f, 0.77f,		1.0f, 0.0f, 0.0f,
			1.0f, 1.0f, 0.0f,			1.0f, 0.0f, 1.0f, 1.0f,			0.675f, 0.14f,		1.0f, 0.0f, 0.0f,
		};

		int a = 0; int b = 1; int c = 2;
		for (int i = 0; i < 6; i++) {
			cube.indices.push_back(a);
			cube.indices.push_back(b);
			cube.indices.push_back(c);
			b++; c++;
			cube.indices.push_back(a);
			cube.indices.push_back(b);
			cube.indices.push_back(c);
			a += 4; b += 3; c += 3;
		}

		cube.floatsPerVertex = 3;
		cube.floatsPerColor = 4;
		cube.floatsPerUV = 2;
		cube.floatsPerNormal = 3;

		return cube;
	}

	OpenGLMesh Plane() {

		OpenGLMesh plane;

		plane.vertices = {

			0.0f, 0.0f, 0.0f,			1.0f, 1.0f, 1.0f, 1.0f,		0.0f, 0.0f,		0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, -1.0f,			1.0f, 1.0f, 1.0f, 1.0f,		1.0f, 0.0f,		0.0f, 1.0f, 0.0f,
			1.0f, 0.0f, -1.0f,			1.0f, 1.0f, 1.0f, 1.0f,		1.0f, 1.0f,		0.0f, 1.0f, 0.0f,
			1.0f, 0.0f, 0.0f,			1.0f, 1.0f, 1.0f, 1.0f,		0.0f, 1.0f,		0.0f, 1.0f, 0.0f,
		};

		plane.indices = {

			0, 1, 3, // base 1
			1, 2, 3, // base 2
		};

		plane.floatsPerVertex = 3;
		plane.floatsPerColor = 4;
		plane.floatsPerUV = 2;
		plane.floatsPerNormal = 3;

		return plane;
	}

	OpenGLMesh Cylinder(int numSides) {

		OpenGLMesh cylinder;

		cylinder.floatsPerVertex = 3;
		cylinder.floatsPerColor = 0;
		cylinder.floatsPerUV = 2;
		cylinder.floatsPerNormal = 3;

		// each slice angle is number of sides into 360
		float spokeAngle = (2 * 3.14159f) / numSides;

		// build a range of angles to work with
		std::vector<float> range;
		for (int i = 0; i <= numSides; i++) {
			range.push_back(i * spokeAngle);
		}

		//build top and bottom of cylinder
		std::vector<float> botVertices = { 0,0,0 }; // center point, bottom
		botVertices.push_back(0.5f); // u
		botVertices.push_back(0.5f); // v (center of texture)
		append(botVertices, DOWN_NORM);

		std::vector<float> topVertices = { 0,1,0 }; // center point, top
		topVertices.push_back(0.5f);
		topVertices.push_back(0.5f);
		append(topVertices, UP_NORM);

		for (auto angle : range) {

			//vertex top
			botVertices.push_back(sin(angle));
			botVertices.push_back(0);
			botVertices.push_back(cos(angle));
			//uv
			botVertices.push_back((sin(angle) + 1.0f) / 2.0f); // convert (-1 to 1) to (0 to 1)
			botVertices.push_back((cos(angle) + 1.0f) / 2.0f);
			//normal bottom
			append(botVertices, DOWN_NORM);

			//vertex bottom
			topVertices.push_back(sin(angle));
			topVertices.push_back(1);
			topVertices.push_back(cos(angle));
			//uv                  // sin returns (-1 to 1) so convert to (0 to 1)
			topVertices.push_back((sin(angle) + 1.0f) / 2.0f); 
			topVertices.push_back((cos(angle) + 1.0f) / 2.0f);
			//normal top
			append(topVertices, UP_NORM);
		}
		
		// build side vertices of cylinder
		int index = 1; // start point for edge vertices (after top and bottom center)
		int stride = cylinder.floatsPerColor 
					+ cylinder.floatsPerNormal 
					+ cylinder.floatsPerUV  
					+ cylinder.floatsPerVertex; // number of items in one group
		index *= stride;
		std::vector<float> sideVertices;

		for (int i = 0; i <= numSides; i++) {

			float u = (float)i / numSides; // chop texture into numSides lengths on u axis

			// get vertex information
			sideVertices.push_back(botVertices.at(index));
			sideVertices.push_back(botVertices.at(index + 1));
			sideVertices.push_back(botVertices.at(index + 2));
			// add uv information
			sideVertices.push_back(u);
			sideVertices.push_back(0);
			// add normal information (its the same as the vertex info)
			sideVertices.push_back(botVertices.at(index));
			sideVertices.push_back(botVertices.at(index + 1));
			sideVertices.push_back(botVertices.at(index + 2));


			// repeat for top
			sideVertices.push_back(topVertices.at(index));
			sideVertices.push_back(topVertices.at(index + 1));
			sideVertices.push_back(topVertices.at(index + 2));
			sideVertices.push_back(u);
			sideVertices.push_back(1);
			sideVertices.push_back(botVertices.at(index));
			sideVertices.push_back(botVertices.at(index + 1));
			sideVertices.push_back(botVertices.at(index + 2));
			
			index += stride; // move to next vertex/uv group
		}
		cylinder.vertices = botVertices;
		append(cylinder.vertices, topVertices);
		append(cylinder.vertices, sideVertices);


		// define indices for drawing triangles 
		// (i think we can skip this and call draw array to make an empty vertex combo and texture it anyway?)
		std::vector<unsigned short> indices;

		// top and bottom triangles
		index = 1;  // start point for edge vertices (after center)
		for (int i = 0; i < 2; i++) {
			for (int j = index; j < numSides + index; j++) {
				indices.push_back(index - 1); //center
				indices.push_back(j); //e1
				indices.push_back(j + 1); //e2
			}
			index += numSides + 2; // switch to top edges for next loop 
		}

		// side triangles 
		// set initial pos
		int start = ((numSides + 1) * 2) + 2; // after top and bottom circle arrays
		int b1 = start;
		int b2 = b1 + 2;
		int t1 = b1 + 1;
		int t2 = t1 + 2; // data at these locations goes {b1}{t1}{b2}{t2}...

		for (int i = start; i <= numSides + start; i++) {

			indices.push_back(b1); 
			indices.push_back(t1); 
			indices.push_back(b2); 

			indices.push_back(t1); 
			indices.push_back(b2); 
			indices.push_back(t2); 

			b1 += 2; t1 += 2; b2 += 2; t2 += 2;
		}
		cylinder.indices = indices;


		// finish
		return cylinder;
	}

	OpenGLMesh Sphere(int numSlices, int numStacks) {

		//ref https://www.mbsoftworks.sk/tutorials/opengl4/022-cylinder-and-sphere/
		// I changed the indices generation for the sides of the sphere as we are not using triangle_strip
		// i didnt want to complicate the render function so much, although I think doing so can save gpu memory
		// the rest follows 

		OpenGLMesh sphere;

		sphere.floatsPerVertex = 3;
		sphere.floatsPerColor = 0;
		sphere.floatsPerUV = 2;
		sphere.floatsPerNormal = 3;
		
		// divide vertical slices into angles of a circle
		float sliceAngle = (2 * 3.14159f) / numSlices;
		std::vector<float> sliceAngles;
		for (int i = 0; i <= numSlices; i++) {
			sliceAngles.push_back(i * sliceAngle);
		}
		
		// divide horizontal stacks into angles of a half circle
		float stackAngle = 3.14159f / 2; // start at +90 deg
		float step = -3.14159f / numStacks;
		std::vector<float> stackAngles;
		for (int i = 0; i <= numStacks; i++) {
			stackAngles.push_back(stackAngle);
			stackAngle += step;
		}

		// build vertices and add uv for textures and normals for lighting
		// this is a circle of circles, outer loop is up to down and inner loop is horizontal
		int i = 0;
		for (auto st_angle : stackAngles) {

			float v = (float)i / numStacks;

			int j = 0;
			for (auto sl_angle : sliceAngles) {

				float u = (float)j / numSlices;

				// a point
				sphere.vertices.push_back(cos(st_angle) * cos(sl_angle));
				sphere.vertices.push_back(sin(st_angle));
				sphere.vertices.push_back(cos(st_angle) * sin(sl_angle));
				// a uv map
				sphere.vertices.push_back(u);
				sphere.vertices.push_back(v);
				// a normal vector (faces outwards perpendicular to the surface)
				// since this vector would start at the center of the sphere and point
				// out trhough each vertex, it is actually equal to the vertex data for each point on a unit sphere
				sphere.vertices.push_back(cos(st_angle) * cos(sl_angle));
				sphere.vertices.push_back(sin(st_angle));
				sphere.vertices.push_back(cos(st_angle) * sin(sl_angle));

				j++;
			}
			i++;
		}

		// build indices
		// north pole
		int numVertices = (numStacks + 1) * (numSlices + 1);
		for (int i = 0; i < numSlices; i++) {

			int nextIndex = i + numSlices + 1;

			sphere.indices.push_back(i);
			sphere.indices.push_back(nextIndex);
			sphere.indices.push_back(nextIndex + 1);
		}

		// south pole
		int startIndex = numVertices - 2 * (numSlices + 1);
		for (int i = 0; i < numSlices; i++) {
			
			int index = startIndex + i;
			int nextIndex = index + numSlices + 1;

			sphere.indices.push_back(index);
			sphere.indices.push_back(index + 1);
			sphere.indices.push_back(nextIndex);
		}

		// rest of sphere
		int t1 = numSlices + 1;
		int t2 = t1 + 1;
		int b1 = t1 + numSlices + 1;
		int b2 = b1 + 1;

		for (int i = 0; i < numStacks; i++) {

			for (int j = 0; j < numSlices; j++) {

				sphere.indices.push_back(t1);
				sphere.indices.push_back(t2);
				sphere.indices.push_back(b1);

				sphere.indices.push_back(t2);
				sphere.indices.push_back(b1);
				sphere.indices.push_back(b2);

				t1++; t2++; b1++; b2++;
			}
		}

		return sphere;
	}

}

#endif
