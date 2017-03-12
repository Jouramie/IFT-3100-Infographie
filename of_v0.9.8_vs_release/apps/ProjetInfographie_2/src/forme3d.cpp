#include "forme3d.h"
#include <algorithm>

forme3d::forme3d() : forme3d{ nullptr, ofColor(255, 255, 255), ofVec3f(1, 1, 1) }
{

}

forme3d::forme3d(of3dPrimitive* primitive) : forme3d{ primitive, ofColor(255, 255, 255), ofVec3f(1, 1, 1) }
{

}

forme3d::forme3d(of3dPrimitive* primitive, ofColor fill) : forme3d{ primitive, fill, ofVec3f(1, 1, 1) }
{

}

forme3d::forme3d(of3dPrimitive* primitive, ofColor fill, ofVec3f scal) : primitive{}, prims{ primitive }, fillCol{ fill }, scale{ scal }
{

}

void forme3d::draw(bool wireframe) {

	ofSetColor(fillCol);
	ofScale(scale.x, scale.y, scale.z);

	if (wireframe || selected)
		for (auto& i : prims) {
			i->drawWireframe();
		}
	else
		for (auto& i : prims) {
			i->draw();
		}

	ofScale(1 / scale.x, 1 / scale.y, 1 / scale.z);
}
