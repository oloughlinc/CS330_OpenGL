#ifndef SHADERS_H
#define SHADERS_H

// Macro for shader programs
#define GLSL(Version, Source) "#version " #Version " core \n" #Source



// Vertex Shader Source
const char* vertexShaderSource = // a vertex shader determines where to place pixels on the screen itself
								// here we multiply every vertex by some model, view, projection
								// and this determines each location

GLSL(440,

	layout(location = 0) in vec3 aPos;
	layout(location = 1) in vec4 colorFromVBO;

	out vec4 colorFromVS;

	uniform mat4 model;
	uniform mat4 view;
	uniform mat4 projection;

	void main()
	{
		gl_Position = projection * view * model * vec4(aPos, 1.0f); // here we create the MVP
		colorFromVS = colorFromVBO; // here we pass color information through this shader
	}
);

// Fragment Shader Source
const char* fragmentShaderSource =

GLSL(440,

	in vec4 colorFromVS; // receive color info from mesh
	out vec4 FragColor;

	void main()
	{
		FragColor = colorFromVS;
	}
);

#endif
