#pragma once

#include "ofxRay.h"

class primitive
{
public:
	primitive() { selected = ofParameter<bool>(false); };

	virtual void draw() { draw(false); }
	virtual void draw(bool wireframe) = 0;

	bool getSelected() { return selected; }
	void setSelected(bool selected) { this->selected = selected; }
	void changeSelected() {	selected = !selected; }

	string getName() { return name; }
	void setName(string n) {
		name = n;
		selected.setName(n);
	}

	ofMatrix4x4 getTransfo() { return transfoMatrix; }
	void setTransfo(ofMatrix4x4 matrix) { this->transfoMatrix = matrix; }

	virtual bool checkIntersectionPlaneAndLine(ofRay ray, float *result) const { return false; };
	ofParameter<bool> selected;

protected:
	ofMatrix4x4 transfoMatrix;
	string name;
};

