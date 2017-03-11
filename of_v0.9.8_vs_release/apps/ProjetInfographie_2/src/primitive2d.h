#pragma once

#include "ofMain.h"

class primitive2d
{
public:

	primitive2d();
	primitive2d(ofPath* primitive);
	primitive2d(ofPath* primitive, ofColor fill);
	ofPath* getPrimitive2d();
	ofColor getFillColor();

private:

	ofPath* prim;
	ofColor fillCol;

};
