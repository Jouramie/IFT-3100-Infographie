#include "forme3d.h"
#include <algorithm>

forme3d::forme3d() : forme3d{ nullptr }
{

}

forme3d::forme3d(of3dPrimitive* primitive) : forme3d{ primitive, ofColor(255, 255, 255) }
{

}

forme3d::forme3d(of3dPrimitive* primitive, ofColor fill) : forme3d{ primitive, fill, ofMatrix4x4() }
{

}

forme3d::forme3d(of3dPrimitive* primitive, ofColor fill, ofMatrix4x4 matrix) : primitive{ matrix }, prims{ primitive }, fillCol{ fill } 
{

}

void forme3d::draw(bool wireframe) {

	ofPushMatrix();
	ofTranslate(transfoMatrix.getTranslation());
	ofSetColor(fillCol);

	ofQuaternion rotation = transfoMatrix.getRotate();
	float rotationAmount;
	ofVec3f rotationAngle;
	rotation.getRotate(rotationAmount, rotationAngle);

	ofRotate(rotationAmount, rotationAngle.x, rotationAngle.y, rotationAngle.z);

	ofScale(transfoMatrix.getScale());

	if (wireframe || selected)
		for (auto& i : prims) {
			i->drawWireframe();
		}
	else
		for (auto& i : prims) {
			i->draw();
		}

	ofPopMatrix();
}
