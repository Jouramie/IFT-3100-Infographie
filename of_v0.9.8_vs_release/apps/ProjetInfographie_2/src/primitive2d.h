#pragma once

#include "ofMain.h"
#include "primitive.h"

class primitive2d : public primitive {
public:

	primitive2d();
	primitive2d(ofPath* primitive);
	primitive2d(ofPath* primitive, ofColor fill);
	primitive2d(ofPath* primitive, ofColor fill, ofColor stroke);
	ofPath* getPrimitive2d();
	ofColor getFillColor();
	ofColor getStrokeColor();

	void draw(bool wireframe) override;

private:

	ofPath* prim;
	ofColor fillCol;
	ofColor strokeCol;

};
