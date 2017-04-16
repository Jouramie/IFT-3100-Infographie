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

	void setMirror(bool mirror);
	void setGlass(bool glass);

	bool prepareGlass(const ofCamera &cam);// , const scene* scn);
	void draw(bool wireframe) override;
	bool intersectsMeshInstance(const ofVec2f &screenCoordinates, const ofCamera &cam) override;
	bool getColorOfRay(ofRay ray, ofColor * hit) override;

protected:
	const ofVec3f getLocalPosition() const override { return prim->getPosition(); }
private:

	of3dPrimitive* prim;
	ofColor fillCol;
	bool isMirror;
	bool isGlass;
	//ofVec3f scale;

};