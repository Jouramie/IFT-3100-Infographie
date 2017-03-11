#pragma once

#include "ofxRay.h"

class primitive
{
public:
	primitive();

	virtual void draw() { draw(false); }
	virtual void draw(bool wireframe) = 0;

	bool getSelected() { return selected; }
	void setSelected(bool selected) { this->selected = selected; }
	void changeSelected() {	selected = !selected; }

	virtual bool checkIntersectionPlaneAndLine(ofRay ray, float *result) const { return false; };
protected:
	bool selected;
};

