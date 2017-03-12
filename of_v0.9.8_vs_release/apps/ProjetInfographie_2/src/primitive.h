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

	string getName() { return name; }
	void setName(string n) { name = n; }

	ofMatrix4x4 getTransfo() { return transfoMatrix; }
	void setTransfo(ofMatrix4x4 matrix) { this->transfoMatrix = matrix; }

	virtual bool checkIntersectionPlaneAndLine(ofRay ray, float *result) const { return false; };
protected:
	bool selected;
	ofMatrix4x4 transfoMatrix;
	string name;
};

