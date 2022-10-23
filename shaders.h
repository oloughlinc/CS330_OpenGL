#ifndef SHADERS_H
#define SHADERS_H

// Macro for shader programs
#define GLSL(Version, Source) "#version " #Version " core \n" #Source



// Vertex Shader Source
const char* vertexShaderSource = // a vertex shader determines where to place pixels on the 2d screen itself
								// here we multiply every vertex by some transforms, model, view, projection
								// and this determines each location

GLSL(440,

	// inputs to shader set by opengl calls
	layout(location = 0) in vec3 aPos;
	layout(location = 1) in vec4 colorFromVBO;
	layout(location = 2) in vec2 textureCoordinate;
	layout(location = 3) in vec3 normal;

	// outputs -> to fragment shader
	out vec4 colorFromVS;
	out vec2 vertexTextureCoordinate;
	out vec3 vertexNormal;
	out vec3 vertexFragmentPosition;

	// uniforms do not change per pixel
	uniform mat4 model;
	uniform mat4 rotation; // the rotation value for the current model, set at render for each
	uniform mat4 view;
	uniform mat4 projection;

	void main()
	{
		gl_Position = projection * view * model * vec4(aPos, 1.0f);
		vertexFragmentPosition = vec3(model * vec4(aPos, 1.0f));

		// rotate normals based on model rotation here, because this is much easier than doing it in the mesh on CPU time
		vec4 normal4 = vec4(normal, 1.0f); // convert to 4d to match mat4
		normal4 *= rotation;
		vec3 rNormal = normal4.xyz; // cast back to vec3 to match rest of code

		vertexNormal = mat3(transpose(inverse(model))) * rNormal; // normals in world space (no view)
		colorFromVS = colorFromVBO; 
		vertexTextureCoordinate = textureCoordinate;
	}
);

// Fragment Shader Source : COLORS
const char* colorShaderSource =

GLSL(440,

	in vec4 colorFromVS; 
    //in vec2 vertexTextureCoordinate;

	out vec4 FragColor;
	//uniform sampler2D uTexture;

	void main()
	{
		FragColor = colorFromVS;
		//FragColor = texture(uTexture, vertexTextureCoordinate);
	}
);

// Fragment Shader Source : TEXTURES
// multiple light sources available besides post processing in a seperate shader 
const char* textureShaderSource =

GLSL(440,

	//in vec4 colorFromVS;
    in vec2 vertexTextureCoordinate;
	in vec3 vertexNormal;
	in vec3 vertexNormalPosition;
	in vec3 vertexFragmentPosition;

	uniform vec3 lightColor1;
	uniform vec3 lightPos1;
	uniform float lightStrength1;

	uniform vec3 lightColor2;
	uniform vec3 lightPos2;
	uniform float lightStrength2;

	uniform vec3 viewPosition;

	uniform float shininess; // this is the per material shininess

    out vec4 FragColor;
    uniform sampler2D uTexture;

void main()
{

	// Phong lighting model calculations to generate ambient, diffuse, and specular components

	// Calculate Ambient lighting	
	float ambientStrength = 0.2f; // Set ambient or global lighting strength
	vec3 ambientColor = vec3(1.0f, 1.0f, 1.0f);
	vec3 ambient = ambientStrength * ambientColor; // Generate ambient light color
	
	// Calculate Diffuse lighting
	vec3 norm = normalize(vertexNormal); // Normalize vectors to 1 unit
	vec3 lightDirection1 = normalize(lightPos1 - vertexFragmentPosition);
	float impact1 = max(dot(norm, lightDirection1), 0.0);
	vec3 lightDirection2 = normalize(lightPos2 - vertexFragmentPosition); 
	float impact2 = max(dot(norm, lightDirection2), 0.0);
	vec3 diffuse = impact1 * lightColor1 * lightStrength1 + impact2 * lightColor2 * lightStrength2; // Generate diffuse light color for two sources

	// Calculate Specular lighting
	float specularIntensity1 = 0.8f * lightStrength1 * shininess; // Set specular light strength
	float specularIntensity2 = 0.8f * lightStrength2 * shininess;
	float highlightSize = 16.0f * shininess; // Set specular highlight size.
	vec3 viewDir = normalize(viewPosition - vertexFragmentPosition); // Calculate view direction
	vec3 reflectDir1 = reflect(-lightDirection1, norm); // Calculate reflection vector light 1
	vec3 reflectDir2 = reflect(-lightDirection2, norm); // Calculate reflection vector light 2
	// Calculate specular component
	float specularComponent1 = pow(max(dot(viewDir, reflectDir1), 0.0), highlightSize);
	float specularComponent2 = pow(max(dot(viewDir, reflectDir2), 0.0), highlightSize);

	// final specular map combines both light views by addition
	vec3 specular = specularIntensity1 * specularComponent1 * lightColor1 + specularIntensity2 * specularComponent2 * lightColor2;

	// Texture holds the color to be used for all three components
	//vec4 textureColor = texture(uTexture, vertexTextureCoordinate * uvScale);
	vec4 textureColor = texture(uTexture, vertexTextureCoordinate * vec2(1.0f, 1.0f));

	// Calculate phong result
	vec3 phong = (ambient + diffuse + specular) * textureColor.xyz;

	// send
	FragColor = vec4(phong, 1.0); 
}
);
#endif
