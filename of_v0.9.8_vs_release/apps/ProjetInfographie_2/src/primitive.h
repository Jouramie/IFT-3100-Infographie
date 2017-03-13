#pragma once

#include "ofxRay.h"

class primitive
{
public:
	primitive() : primitive{ ofMatrix4x4() } { }
	primitive(ofMatrix4x4 matrix) : transfoMatrix{ matrix }, selected{ false } { }

	virtual void draw() { draw(false); }
	virtual void draw(bool wireframe) = 0;
	const ofVec3f getGlobalPosition() const;

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
	virtual const ofVec3f getLocalPosition() const = 0;
	ofMatrix4x4 transfoMatrix;
	string name;
};

