#pragma once

#include "ofMain.h"
#include "ofxBezierSurface.h"
#include "primitive.h"

class primitiveTopo : public primitive {
public:

	primitiveTopo();
	primitiveTopo(ofxBezierSurface* primitive);
	primitiveTopo(ofxBezierSurface* primitive, ofColor fill);
	primitiveTopo(ofxBezierSurface* primitive, ofColor fill, ofColor stroke);
	primitiveTopo(ofxBezierSurface* primitive, ofColor fill, ofParameter<float> strokeThickness);
	primitiveTopo(ofxBezierSurface* primitive, ofColor fill, ofColor stroke, ofParameter<float> strokeThickness);
	ofxBezierSurface* getprimitiveTopo();
	ofColor getFillColor();
	ofColor getStrokeColor();

	void draw(bool wireframe, ofxShadersFX::Lighting::LightingShader& lightShader) override;

protected:
	const ofVec3f getLocalPosition() const override { return ofVec3f(); } //Fix later

private:
	ofxBezierSurface* prim;
	ofColor fillCol;
	ofColor strokeCol;
	float strokeThickness = 3;
};
