#pragma once

#include "ofMain.h"
#include "ofxRay.h"
#include <algorithm>

class primitive
{
public:
	
	primitive();
	primitive(of3dPrimitive* primitive);
	primitive(of3dPrimitive* primitive, ofColor fill);
	of3dPrimitive* getPrimitive();
	ofColor getFillColor();
	void draw(bool wireframe = false);
	bool calcTriangleIntersection(ofRay ray, float *result) const;
	bool getSelected();
	void setSelected(bool val);
	void changeSelected();
	bool inside(ofVec3f p, float xmin, float xmax, float ymin, float ymax, float zmin, float zmax) const;
	bool checkIntersectionPlaneAndLine(ofRay ray, float *result) const;

private:

	of3dPrimitive* prim;
	ofColor fillCol;
	bool selected;

};