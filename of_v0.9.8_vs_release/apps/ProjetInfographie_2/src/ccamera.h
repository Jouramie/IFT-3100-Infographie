#pragma once

#include "ofMain.h"

class ccamera
{
public:

	const float MaxX = 5000.0f, MaxY = 5000.0f, MaxZ = 5000.0f;
	const float MinX = -5000.0f, MinY = -5000.0f, MinZ = -5000.0f;

	ccamera() : ccamera{ nullptr } {};
	ccamera(const ccamera&) = default;
	ccamera(ofEasyCam* cam) : cam{ cam } { }

	ofParameterGroup getParameterGroup() { return parameterGroup; }
	void setSpeed(float speed) { this->speed = speed; }
	float getSpeed() const { return speed; }
	void setCamera(ofEasyCam* cam) { this->cam = cam; }
	void setTarget(const ofVec3f& vec) { cam->setTarget(vec); }
	ofVec3f getTarget() { return cam->getTarget().getPosition(); }

	void setup() { setupCamera(); setupParameters(); }
	void update(float dt);

	float getPosX() { return posX.get(); };
	float getPosY() { return posY.get(); };
	float getPosZ() { return posZ.get(); };

	void begin() { cam->begin(); }
	void end() { cam->end(); }

	bool compare(ccamera otherCam);
	
	bool isCameraMoveLeft = false;
	bool isCameraMoveRight = false;
	bool isCameraMoveUp = false;
	bool isCameraMoveDown = false;
	bool isCameraMoveForward = false;
	bool isCameraMoveBackward = false;

	const ofEasyCam& operator*() const { return *cam; }
	ofCamera* getOfCamera() { return cam; };

	ccamera& operator=(const ccamera&) = default;

private:

	ofEasyCam* cam;

	float speed;

	void setupCamera();
	void setupParameters();

	ofParameterGroup parameterGroup;
	ofParameter<float> posX;
	ofParameter<float> posY;
	ofParameter<float> posZ;

	ofParameter<float> fov;
	ofParameter<bool> autoRatio;
	ofParameter<float> ratio;

	ofParameter<float> nearClip;
	ofParameter<float> farClip;

	ofParameter<bool> ortho;
};

