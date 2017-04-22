#pragma once

#include "ofMain.h"
#include "primitive.h"

class primitive2d : public primitive {
public:

	primitive2d();
	primitive2d(ofPath* primitive);
	primitive2d(ofPath* primitive, ofColor fill);
	primitive2d(ofPath* primitive, ofColor fill, ofColor stroke);
	primitive2d(ofPath* primitive, ofColor fill, ofParameter<float> strokeThickness);
	primitive2d(ofPath* primitive, ofColor fill, ofColor stroke, ofParameter<float> strokeThickness);
	ofPath* getPrimitive2d();
	ofColor getFillColor();
	ofColor getStrokeColor();

	void draw(bool wireframe) override;

	vector<hit> intersectsMeshInstance(const ofVec2f &screenCoordinates, const ofCamera &cam) override;

protected:
	const ofVec3f getLocalPosition() const override { return ofVec3f(); } //Fix later

private:
	ofPath* prim;
	ofColor fillCol;
	ofColor strokeCol;
	float strokeThickness = 3;
};
