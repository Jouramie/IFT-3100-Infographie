#pragma once

#include "ofMain.h"

class ccamera
{
public:

	const float MaxX = 2000, MaxY = 1000, MaxZ = 1000;
	const float MinX = 0, MinY = 0, MinZ = -1000;


	ccamera();
	~ccamera();

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

