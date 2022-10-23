#ifndef LIGHT_H
#define LIGHT_H

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Light {

	private:

		glm::vec3 mPosition;		
		glm::vec3 mColor;
		float mIntensity;
		
	public:

		Light() {

			setColor(1.0f, 1.0f, 1.0f);
			setPosition(0.0f, 0.0f, 0.0f);
			setIntensity(0.0f);
		
		};

		Light* setColor(float R, float G, float B) { 
			mColor = glm::vec3(R, G, B); 
			this->R = mColor.r;
			this->G = mColor.g;
			this->B = mColor.b;
			return this;
		}
		glm::vec3 getColor() { return mColor; }
		float R;
		float G;
		float B;

		Light* setIntensity(float intensity) { 
			mIntensity = intensity; 
			return this; 
		}
		float getIntensity() { return mIntensity; }

		Light* setPosition(float X, float Y, float Z) { 
			mPosition = glm::vec3(X, Y, Z);
			this->X = mPosition.x;
			this->Y = mPosition.y;
			this->Z = mPosition.z;
			return this;
		}
		glm::vec3 getPosition() { return mPosition; }
		float X;
		float Y;
		float Z;

};

#endif
