#pragma once

#include "ofMain.h"
#include "primitive.h"

class primitive3d : public primitive {
public:
	
	primitive3d();
	primitive3d(of3dPrimitive* primitive);
	primitive3d(of3dPrimitive* primitive, ofColor fill);
	primitive3d(of3dPrimitive* primitive, ofColor fill, ofMatrix4x4 matrix);
	of3dPrimitive* getPrimitive();
	ofColor getFillColor();
	void draw(bool wireframe) override;
	bool calcTriangleIntersection(ofRay ray, float *result) const;

	bool inside(ofVec3f p, float xmin, float xmax, float ymin, float ymax, float zmin, float zmax) const;
	bool checkIntersectionPlaneAndLine(ofRay ray, float *result) const override;
	bool checkIntersectionTriangleRay(ofRay ray, ofPoint* inter);

private:

	of3dPrimitive* prim;
	ofColor fillCol;
	//ofVec3f scale;

};