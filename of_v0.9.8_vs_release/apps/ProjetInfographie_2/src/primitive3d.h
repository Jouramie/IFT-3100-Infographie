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

	vector<ofRay> prepareGlass(const ofCamera cam, vector<primitive*> otherPrims, ofColor backgroundCol) override;// , const scene* scn);
	void draw(bool wireframe) override;
	vector<hit> intersectsMeshInstance(const ofVec2f &screenCoordinates, const ofCamera &cam) override;
	bool getColorOfRay(ofRay ray, ofColor * hit) override;

	ofMesh getMesh() override { return prim->getMesh(); };

	void shouldPrepare() override { mustPrepare = true; };

	bool isGlassy() override { return isMirror || isGlass; } ;
	bool isCubeOrSphere() override { return true; };

protected:
	const ofVec3f getLocalPosition() const override { return prim->getPosition(); }
private:
	bool mustPrepare;
	of3dPrimitive* prim;
	ofColor fillCol;
	bool isMirror;
	bool isGlass;
	ofImage toPaint;
	//ofVec3f scale;

};