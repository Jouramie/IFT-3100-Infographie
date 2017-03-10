#pragma once

#include "ofMain.h"

class camera
{
public:

	const float MaxX = 2000, MaxY = 1000, MaxZ = 1000;
	const float MinX = 0, MinY = 0, MinZ = -1000;


	camera();
	~camera();

	ofParameterGroup getParameterGroup();

private:


	ofParameterGroup parameterGroup;
	ofParameter<float> posX;
	ofParameter<float> posY;
	ofParameter<float> posZ;

	ofParameter<float> fovH;
	ofParameter<float> fovV;
	ofParameter<float> aspectRatio;

	ofParameter<float> frontClippingPlan;
	ofParameter<float> backClippingPlan;

};

