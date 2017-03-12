#pragma once

#include "ofMain.h"
#include "primitive.h"

class forme3d : public primitive {
public:

	forme3d();
	forme3d(of3dPrimitive* prim);
	forme3d(of3dPrimitive* prim, ofColor fill);
	forme3d(of3dPrimitive* prim, ofColor fill, ofVec3f scal);
	const std::vector<of3dPrimitive*>& getPrimitives() { return prims; }
	void addPrimitive(of3dPrimitive* prim) { prims.push_back(prim); }
	ofColor getFillColor() { return fillCol; }
	void draw(bool wireframe) override;

private:

	std::vector<of3dPrimitive*> prims;
	ofColor fillCol;
	ofVec3f scale;

};