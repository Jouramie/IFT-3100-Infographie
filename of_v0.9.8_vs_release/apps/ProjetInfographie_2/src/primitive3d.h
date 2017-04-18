#pragma once

#include "ofMain.h"
#include "primitive.h"
#include "ccamera.h"

class primitive3d : public primitive {
public:

	primitive3d();
	primitive3d(of3dPrimitive* primitive);
	primitive3d(of3dPrimitive* primitive, ofColor fill);
	primitive3d(of3dPrimitive* primitive, ofColor fill, ofMatrix4x4 matrix);
	of3dPrimitive* getPrimitive();
	ofColor getFillColor();
	void draw(bool wireframe, ofxShadersFX::Lighting::LightingShader& lightShader) override;
	bool intersectsMeshInstance(const ofVec2f &screenCoordinates, const ofCamera &cam) override;

protected:
	const ofVec3f getLocalPosition() const override { return prim->getPosition(); }
private:

	of3dPrimitive* prim;
	ofColor fillCol;
	//ofVec3f scale;

};