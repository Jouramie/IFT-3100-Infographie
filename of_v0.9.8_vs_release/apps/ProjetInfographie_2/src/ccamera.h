#pragma once

#include "ofMain.h"

class ccamera
{
public:

	const float MaxX = 2000.0f, MaxY = 2000.0f, MaxZ = 2000.0f;
	const float MinX = -2000.0f, MinY = -2000.0f, MinZ = -2000.0f;

	ccamera() : ccamera{ nullptr } {}
	ccamera(ofCamera* cam) : cam{ cam } { }

	ofParameterGroup getParameterGroup() { return parameterGroup; }
	void setSpeed(float speed) { this->speed = speed; }
	float getSpeed() const { return speed; }
	void setCamera(ofCamera* cam) { this->cam = cam; }

	void setup() { setupCamera(); setupParameters(); }
	void update(float dt);

	void begin() { cam->begin(); }
	void end() { cam->end(); }
	
	bool isCameraMoveLeft = false;
	bool isCameraMoveRight = false;
	bool isCameraMoveUp = false;
	bool isCameraMoveDown = false;
	bool isCameraMoveForward = false;
	bool isCameraMoveBackward = false;

	void changeMode();

	void resetPos();

	const ofCamera& operator*() const { return *cam; }

private:

	ofCamera* cam;

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
	ofParameter<bool> camInteractive;

};

