#pragma once

#include "ofMain.h"
#include "ofxRay.h"
#include <algorithm>
#include "ofxAssimpModelLoader.h"

class extModel
{
public:

	extModel();
	extModel(ofxAssimpModelLoader* mod);
	extModel(ofxAssimpModelLoader* mod, ofMatrix4x4 transfoMatrix);
	ofxAssimpModelLoader* getModel();
	void draw(bool wireframe = false);
	void changeSelected();
	bool getSelected();
	void setSelected(bool val);

	string getName() { return name; }
	void setName(string n) { name = n; }

	bool calcTriangleIntersection(ofRay ray, float *result) const;
	bool inside(ofVec3f p, float xmin, float xmax, float ymin, float ymax, float zmin, float zmax) const;
	bool checkIntersectionPlaneAndLine(ofRay ray, float *result) const;
	bool checkIntersectionTriangleRay(ofRay ray, ofPoint* inter);
	ofParameter<bool> selected;

private:

	ofxAssimpModelLoader* model;
	ofMatrix4x4 transfo;
	string name;
};